#include "mallaTVT.hpp"

MallaTVT::MallaTVT(vector<GLfloat> vertices, vector<int> caras)
{

   // Pasamos los vértices a vector de Tupla3f
   for (unsigned i = 0; i < vertices.size(); i+=3)
   {
      ver.push_back(Tupla3f(vertices[i],vertices[i+1],vertices[i+2]));
   }

   // Pasamos las caras a vectores de Tupla3f
   for (unsigned i = 0; i < caras.size(); i+=6)
   {
      Tupla3i cara(caras[i],caras[i+1],caras[i+2]);
      tri.push_back(cara);
   }

   Inicializar();
}

MallaTVT::MallaTVT(vector<Tupla3f> vertices, vector<Tupla3i> caras)
{

   ver = vertices;

   // Pasamos las caras a vectores de Tupla3f
   for (unsigned i = 0; i < caras.size(); i+=2)
   {
      tri.push_back(caras[i]);
   }

   for (unsigned i = 1; i < caras.size(); i+=2)
   {
      tri.push_back(caras[i]);
   }

   Inicializar();
}

void MallaTVT::Inicializar()
{
   CalcularDimension();
   CalcularVectoresNormales();

   // Asignamos colores a los vértices según su normal
   for (unsigned i = 0; i < ver.size(); i++)
   {
      Tupla3f color(normales_vertices[i].abs());
      colores_vertices.push_back(color);
   }

   this->modo_dibujo = ALAMBRE;
   this->dibujo_normales = NADA;

   CrearVBOs();
}

void MallaTVT::CalcularDimension()
{
   float dimension_maxima_ejes[3] = {0.0,0.0,0.0};
   float dimension_minima_ejes[3] = {0.0,0.0,0.0};

   for (unsigned vertice = 0; vertice < ver.size(); vertice++)
   {
      for (unsigned coo = 0; coo < 3; coo++)
      {
         if (ver[vertice][coo] < dimension_minima_ejes[coo])
            dimension_minima_ejes[coo] = ver[vertice][coo];
         else if (ver[vertice][coo] > dimension_maxima_ejes[coo])
            dimension_maxima_ejes[coo] = ver[vertice][coo];
      }
   }

   for (unsigned coo = 0; coo < 3; coo++)
   {
         dimension_maxima_ejes[coo] -= dimension_minima_ejes[coo];
   }

   dimension = max(dimension_maxima_ejes[X],max(dimension_maxima_ejes[Y], dimension_maxima_ejes[Z]));
}

void MallaTVT::CalcularVectoresNormales()
{
   // Primero las normales de las caras y baricentros
   Tupla3f baricentro;
   for (unsigned cara = 0; cara < tri.size(); cara++)
   {
      Tupla3f A = ver[tri[cara][X]];
      Tupla3f B = ver[tri[cara][Y]];
      Tupla3f C = ver[tri[cara][Z]];

      Tupla3f AB = B - A;
      Tupla3f BC = C - B;

      Tupla3f normal((AB*BC).normalized());

      normales_caras.push_back(normal);

      baricentro[X] = (A[X] + B[X] + C[X])/3;
      baricentro[Y] = (A[Y] + B[Y] + C[Y])/3;
      baricentro[Z] = (A[Z] + B[Z] + C[Z])/3;
      baricentros.push_back(baricentro);

      pair<Tupla3f,Tupla3f> linea (baricentro, baricentro + normal * 0.1 * dimension);
      lineas_normales_caras.push_back(linea);
   }

   Tupla3f ceros(0.0,0.0,0.0);

   for (unsigned vertice = 0; vertice < ver.size(); vertice++)
   {
      normales_vertices.push_back(ceros);
   }

   // Ahora las normales a los vértices
   for (unsigned cara = 0; cara < tri.size(); cara++)
   {
      unsigned A = tri[cara][X];
      unsigned B = tri[cara][Y];
      unsigned C = tri[cara][Z];

      normales_vertices[A] += normales_caras[cara];
      normales_vertices[B] += normales_caras[cara];
      normales_vertices[C] += normales_caras[cara];
   }

   for (unsigned vertice = 0; vertice < ver.size(); vertice++)
   {
      normales_vertices[vertice] = (normales_vertices[vertice]).normalized();

      pair<Tupla3f,Tupla3f> linea (ver[vertice], ver[vertice] + normales_vertices[vertice] * 0.1 * dimension);
      lineas_normales_vertices.push_back(linea);
   }
}

void MallaTVT::CrearVBOs()
{

   unsigned
      num_verts = ver.size(),
      num_tri = tri.size();

   unsigned
      elementos_vertices = 3L * num_verts,
      elementos_triangulos = 3L * num_tri,   // Los triángulos van en un VBO GL_ELEMENT y se cuentan los vértices, no los índices en sí
      elementos_lineas_normales_caras = 2L * elementos_triangulos,
      elementos_lineas_normales_vertices = 2L * elementos_vertices;


   unsigned
      tam_ver = sizeof(float) * elementos_vertices ,
      tam_tri = sizeof(int) * elementos_triangulos,
      tam_lineas_normales_caras = sizeof(float) * elementos_lineas_normales_caras,
      tam_lineas_normales_vertices = sizeof(float) * elementos_lineas_normales_vertices;

   vbo_vertices = new VBO_Vertices(elementos_vertices, tam_ver, ver.data());
   vbo_triangulos = new VBO_Triangulos(elementos_triangulos, tam_tri, tri.data());
   vbo_colores_vertices = new VBO_Colores(elementos_vertices, tam_ver, colores_vertices.data());
   vbo_normales_vertices = new VBO_Normales(elementos_vertices, tam_ver, normales_vertices.data());

   vbo_lineas_normales_caras = new VBO_Lineas(elementos_lineas_normales_caras, tam_lineas_normales_caras, lineas_normales_caras.data() );
   vbo_lineas_normales_vertices = new VBO_Lineas(elementos_lineas_normales_vertices, tam_lineas_normales_vertices, lineas_normales_vertices.data() );
}

void MallaTVT::Visualizar()
{

   // Ponemos un color por defecto por si no hubiera array de colores
   glColor3f(0.0,1.0,1.0);

   switch (modo_dibujo)
   {
      case ALAMBRE:
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         break;
      case SOLIDO:
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         break;
      case PUNTOS:
         break;
      case AJEDREZ:
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         break;
      case SOLIDO_CARAS:
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         break;
      default:
         cout << "Enumerado inválido para modo_dibujo" << endl;
         exit(-3);
         break;

   }
   if (modo_dibujo == SOLIDO_CARAS)
   {
      VisualizarModoInmediato();
   }
   else
   {
      // Ver si usamos array de colores o vértices
      if (!colores_vertices.empty())
      {
         vbo_colores_vertices->Activar();
      }

      if (!normales_vertices.empty())
      {
         vbo_normales_vertices->Activar();
      }

      vbo_vertices->Activar();

      if (modo_dibujo == PUNTOS)
      {
         vbo_vertices->Visualizar();
      }
      else
      {
         vbo_triangulos->Visualizar(modo_dibujo);
      }

      if (!colores_vertices.empty())
         glDisableClientState( GL_COLOR_ARRAY );
      if (!normales_vertices.empty())
         glDisableClientState( GL_NORMAL_ARRAY );
   }

   if (dibujo_normales == AMBAS || dibujo_normales == CARAS )
   {
      VisualizarNormalesCaras();
   }

   if (dibujo_normales == AMBAS || dibujo_normales == VERTICES)
   {
      VisualizarNormalesVertices();
   }

}

void MallaTVT::VisualizarModoInmediato()
{
   glBegin( GL_TRIANGLES );
   for (unsigned i = 0; i < tri.size(); i++)
   {
      if(!normales_caras.empty())
      {
         glColor3fv( normales_caras[i].abs().data() );
         glNormal3fv( normales_caras[i].data() );
      }

      for (int j = 0; j < 3; ++j) {
         unsigned int iv = tri[i][j]; // iv = índice de vértice
         glVertex3fv(ver[iv].data());
      }
   }
   glEnd();
}

void MallaTVT::VisualizarNormalesCaras()
{
   glColor3f(0.0,0.0,1.0);

   vbo_lineas_normales_caras->Activar();
   vbo_lineas_normales_caras->Visualizar();
}

void MallaTVT::VisualizarNormalesVertices()
{
   glColor3f(1.0,0.0,0.0);

   vbo_lineas_normales_vertices->Activar();
   vbo_lineas_normales_vertices->Visualizar();
}

MallaTVT* MallaTVT::Revolucion(const unsigned caras)
{

   float alpha = 2*M_PI/caras;

   vector<Tupla3f> centro_tapas;


   if (ver.front()[X] != 0.0f)
   {
      centro_tapas.push_back(Tupla3f(0.0,ver.front()[Y],0.0));
   }
   else
   {
      centro_tapas.push_back(ver.front());
      ver.erase(ver.begin());
   }

   if (ver.back()[X] != 0.0f)
   {
      centro_tapas.push_back(Tupla3f(0.0,ver.back()[Y],0.0));
   }
   else
   {
      centro_tapas.push_back(ver.back());
      ver.pop_back();
   }

   unsigned vertices_perfil = ver.size();


   // Crear matriz de perfiles
   vector<vector<Tupla3f> > perfiles;

   perfiles.push_back(ver);
   vector<Tupla3f> perfil_actual;
   vector<int> vertices_fijos;
   Tupla3f vertice_actual;

   for (unsigned perfil = 1; perfil < caras; perfil++)
   {
      perfil_actual.clear();
      for (unsigned i = 0; i < vertices_perfil; i++)
      {
         vertice_actual = Matriz4x4::RotacionEjeY(alpha)*perfiles[perfil-1][i];
         perfil_actual.push_back(vertice_actual);
         ver.push_back(vertice_actual);
      }
      perfiles.push_back(perfil_actual);
   }



   // Añadir triángulos
   for (unsigned perfil = 0; perfil < caras-1; perfil++)
   {
      for (unsigned vertice = 1; vertice < vertices_perfil; vertice++)     // Cogemos los triángulos igual que en el guión de prácticas
      {
         unsigned indice_vertice_actual = perfil * vertices_perfil + vertice;
         unsigned indice_vertice_anterior = indice_vertice_actual - 1;
         unsigned indice_vertice_siguiente_perfil = indice_vertice_actual + vertices_perfil;
         unsigned indice_vertice_anterior_siguiente_perfil = indice_vertice_anterior + vertices_perfil;

         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_anterior_siguiente_perfil));
         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior_siguiente_perfil, indice_vertice_siguiente_perfil));
      }
   }

   // Último perfil a fuego
   unsigned perfil = caras - 1;
   for (unsigned vertice = 1; vertice < vertices_perfil; vertice++)     // Cogemos los triángulos igual que en el guión de prácticas
   {
      unsigned indice_vertice_actual = perfil * vertices_perfil + vertice;
      unsigned indice_vertice_anterior = indice_vertice_actual - 1;
      unsigned indice_vertice_siguiente_perfil = vertice;
      unsigned indice_vertice_anterior_siguiente_perfil = vertice - 1;

      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_anterior_siguiente_perfil));
      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior_siguiente_perfil, indice_vertice_siguiente_perfil));
   }

   // Tapa inferior
   ver.push_back(centro_tapas.front());
   centro_tapas.erase(centro_tapas.begin());

   int centro_tapa_inferior = ver.size() - 1;
   for (unsigned cara = 0; cara < caras - 1; cara++)
   {
      unsigned vertice_actual = cara * vertices_perfil;
      unsigned vertice_siguiente = vertice_actual + vertices_perfil;

      Tupla3i triangulo(centro_tapa_inferior,vertice_siguiente,vertice_actual);
      tri.push_back(triangulo);
   }
   tri.push_back(Tupla3i(centro_tapa_inferior,0,(caras-1)*vertices_perfil));

   // Tapa superior
   ver.push_back(centro_tapas.front());
   centro_tapas.erase(centro_tapas.begin());

   int centro_tapa_superior = centro_tapa_inferior + 1;
   for (unsigned cara = 0; cara < caras - 1; cara++)
   {
      unsigned vertice_actual = (cara + 1) * vertices_perfil - 1;
      unsigned vertice_siguiente = vertice_actual + vertices_perfil;

      Tupla3i triangulo(centro_tapa_superior,vertice_actual,vertice_siguiente);
      tri.push_back(triangulo);
   }

   tri.push_back(Tupla3i(centro_tapa_superior, caras*vertices_perfil - 1,vertices_perfil - 1));

   // Construimos una malla nueva
   MallaTVT *res = new MallaTVT(ver,tri);

   // Borramos la actual
   delete this; // Cuidado! Después de esto NO TOCAR EL PROPIO OBJETO

   // Devolvemos puntero a la malla nueva
   return res;

}

MallaTVT* MallaTVT::Barrido_Rotacion(const unsigned caras)
{

   float alpha = 2*M_PI/caras;

   vector<Tupla3f> centro_tapas;

   unsigned vertices_perfil = ver.size();


   // Crear matriz de perfiles
   vector<vector<Tupla3f> > perfiles;

   perfiles.push_back(ver);
   vector<Tupla3f> perfil_actual;
   vector<int> vertices_fijos;
   Tupla3f vertice_actual;

   for (unsigned perfil = 1; perfil < caras; perfil++)
   {
      perfil_actual.clear();
      for (unsigned i = 0; i < vertices_perfil; i++)
      {
         vertice_actual = Matriz4x4::RotacionEjeY(alpha)*perfiles[perfil-1][i];
         perfil_actual.push_back(vertice_actual);
         ver.push_back(vertice_actual);
      }
      perfiles.push_back(perfil_actual);
   }



   // Añadir triángulos

   unsigned indice_vertice_actual;
   unsigned indice_vertice_anterior;
   unsigned indice_vertice_siguiente_perfil;
   unsigned indice_vertice_anterior_siguiente_perfil;


   for (unsigned perfil = 0; perfil < caras-1; perfil++)
   {
      for (unsigned vertice = 1; vertice < vertices_perfil; vertice++)     // Cogemos los triángulos igual que en el guión de prácticas
      {
         indice_vertice_actual = perfil * vertices_perfil + vertice;
         indice_vertice_anterior = indice_vertice_actual - 1;
         indice_vertice_siguiente_perfil = indice_vertice_actual + vertices_perfil;
         indice_vertice_anterior_siguiente_perfil = indice_vertice_anterior + vertices_perfil;

         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_anterior_siguiente_perfil));
         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior_siguiente_perfil, indice_vertice_siguiente_perfil));
      }

      indice_vertice_actual = perfil * vertices_perfil;
      indice_vertice_anterior = indice_vertice_actual + vertices_perfil - 1;
      indice_vertice_siguiente_perfil = indice_vertice_actual + vertices_perfil;
      indice_vertice_anterior_siguiente_perfil = indice_vertice_anterior + vertices_perfil;

      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_anterior_siguiente_perfil));
      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior_siguiente_perfil, indice_vertice_siguiente_perfil));

   }


   // Último perfil a fuego
   unsigned perfil = caras - 1;
   for (unsigned vertice = 1; vertice < vertices_perfil; vertice++)     // Cogemos los triángulos igual que en el guión de prácticas
   {
      indice_vertice_actual = perfil * vertices_perfil + vertice;
      indice_vertice_anterior = indice_vertice_actual - 1;
      indice_vertice_siguiente_perfil = vertice;
      indice_vertice_anterior_siguiente_perfil = vertice - 1;

      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_anterior_siguiente_perfil));
      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior_siguiente_perfil, indice_vertice_siguiente_perfil));
   }


   // Último rectángulo a fuego
   // El actual es el primero del último perfil
   // El anterior es el último del último perfil
   // El del siguiente perfil es el 0
   // El del siguiente perfil anterior es el último del primer perfil
   indice_vertice_actual = perfil * vertices_perfil;
   indice_vertice_anterior = indice_vertice_actual + vertices_perfil - 1;
   indice_vertice_siguiente_perfil = 0;
   indice_vertice_anterior_siguiente_perfil = vertices_perfil - 1;

   tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_anterior_siguiente_perfil));
   tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior_siguiente_perfil, indice_vertice_siguiente_perfil));

   // Construimos una malla nueva
   MallaTVT *res = new MallaTVT(ver,tri);

   // Borramos la actual
   delete this; // Cuidado! Después de esto NO TOCAR EL PROPIO OBJETO

   // Devolvemos puntero a la malla nueva
   return res;

}

MallaTVT* MallaTVT::Barrido_Traslacion(const unsigned caras, const float dx, const float dy, const float dz)
{

   unsigned vertices_perfil = ver.size();

   // Crear matriz de perfiles
   vector<vector<Tupla3f> > perfiles;

   perfiles.push_back(ver);
   vector<Tupla3f> perfil_actual;
   Tupla3f vertice_actual;

   for (unsigned perfil = 1; perfil < caras; perfil++)
   {
      perfil_actual.clear();
      for (unsigned i = 0; i < vertices_perfil; i++)
      {
         vertice_actual = Matriz4x4::Traslacion(dx,dy,dz)*perfiles[perfil-1][i];
         perfil_actual.push_back(vertice_actual);
         ver.push_back(vertice_actual);
      }
      perfiles.push_back(perfil_actual);
   }



   // Añadir triángulos

   unsigned indice_vertice_actual;
   unsigned indice_vertice_anterior;
   unsigned indice_vertice_siguiente_perfil;
   unsigned indice_vertice_anterior_siguiente_perfil;

   for (unsigned perfil = 0; perfil < caras-1; perfil++)
   {
      for (unsigned vertice = 1; vertice < vertices_perfil; vertice++)     // Cogemos los triángulos igual que en el guión de prácticas
      {
         indice_vertice_actual = perfil * vertices_perfil + vertice;
         indice_vertice_anterior = indice_vertice_actual - 1;
         indice_vertice_siguiente_perfil = indice_vertice_actual + vertices_perfil;
         indice_vertice_anterior_siguiente_perfil = indice_vertice_anterior + vertices_perfil;

         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_anterior_siguiente_perfil));
         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior_siguiente_perfil, indice_vertice_siguiente_perfil));
      }

      indice_vertice_actual = perfil * vertices_perfil;
      indice_vertice_anterior = indice_vertice_actual + vertices_perfil - 1;
      indice_vertice_siguiente_perfil = indice_vertice_actual + vertices_perfil;
      indice_vertice_anterior_siguiente_perfil = indice_vertice_anterior + vertices_perfil;

      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_anterior_siguiente_perfil));
      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior_siguiente_perfil, indice_vertice_siguiente_perfil));

   }

   // Calcular primera tapa
   Tupla3f centro_tapa_primera(0.0,0.0,0.0);

   for (unsigned i = 0; i < vertices_perfil; i++)
      centro_tapa_primera += ver[i];
   centro_tapa_primera = centro_tapa_primera/vertices_perfil;

   // Calcular la última tapa
   Tupla3f centro_tapa_ultima(centro_tapa_primera);
   for (unsigned i = 1; i < caras; i++)
      centro_tapa_ultima = Matriz4x4::Traslacion(dx,dy,dz)*centro_tapa_ultima;


   // Tapa inferior
   ver.push_back(centro_tapa_primera);

   unsigned centro_tapa_inferior = ver.size() - 1;
   for (unsigned cara = 0; cara < vertices_perfil - 1; cara++)
   {
      unsigned vertice_actual = cara ;
      unsigned vertice_siguiente = cara + 1;

      Tupla3i triangulo(centro_tapa_inferior,vertice_siguiente,vertice_actual);
      tri.push_back(triangulo);
   }
   tri.push_back(Tupla3i(centro_tapa_inferior,0,vertices_perfil - 1));

   // Tapa superior
   ver.push_back(centro_tapa_ultima);

   unsigned centro_tapa_superior = centro_tapa_inferior + 1;
   for (unsigned cara = (caras - 1) * vertices_perfil; cara < ver.size() - 3; cara++)
   {
      unsigned vertice_actual = cara ;
      unsigned vertice_siguiente = cara + 1;

      Tupla3i triangulo(centro_tapa_superior,vertice_actual,vertice_siguiente);
      tri.push_back(triangulo);
   }

   tri.push_back(Tupla3i(centro_tapa_superior, caras * vertices_perfil - 1, (caras - 1) * vertices_perfil));

   // Construimos una malla nueva
   MallaTVT *res = new MallaTVT(ver,tri);

   // Borramos la actual
   delete this; // Cuidado! Después de esto NO TOCAR EL PROPIO OBJETO

   // Devolvemos puntero a la malla nueva
   return res;
}

enum visualizacion MallaTVT::getModo()
{
   return modo_dibujo;
}

void MallaTVT::CambioModoDibujo(enum visualizacion modo_dibujo)
{
   this->modo_dibujo = modo_dibujo;
}

void MallaTVT::CambioModoNormales()
{
   if (dibujo_normales == NADA)
      dibujo_normales = CARAS;
   else if (dibujo_normales == CARAS)
      dibujo_normales = VERTICES;
   else if (dibujo_normales == VERTICES)
      dibujo_normales = AMBAS;
   else
      dibujo_normales = NADA;
}
