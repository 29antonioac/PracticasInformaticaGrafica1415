#include "mallaTVT.hpp"



MallaTVT::MallaTVT(std::vector<GLfloat> vertices, enum visualizacion modo, std::vector<int> caras)
{
   unsigned num_verts = vertices.size()/3,
   num_tri = caras.size()/3;

   // Esta división tan rara de triángulos
   // pares e impares se hace para que en caso
   // de número total de triángulos impares no quede
   // ninguno suelto

   unsigned num_pares = num_tri / 2,
   num_impares = num_tri - num_pares;

   ver.resize(num_verts);
   tri.resize(num_tri);
   pares.resize(num_pares);
   impares.resize(num_impares);


   unsigned i,j;


   for ( i = 0, j = 0 ; j < num_verts ; i+=3, j++ )
   {
      ver[j][0] = vertices.at(i);
      ver[j][1] = vertices.at(i+1);
      ver[j][2] = vertices.at(i+2);
   }

   for ( i = 0, j = 0 ; j < num_tri ; i+=3, j++ )
   {
      tri[j][0] = caras.at(i);
      tri[j][1] = caras.at(i+1);
      tri[j][2] = caras.at(i+2);
   }

   for (i = 1, j = 0; i < num_tri; i+=2, j++)
   {
      pares[j][0] = tri[i][0];
      pares[j][1] = tri[i][1];
      pares[j][2] = tri[i][2];
   }


   for (i = 0, j = 0; i < num_tri; i+=2, j++)
   {
      impares[j][0] = tri[i][0];
      impares[j][1] = tri[i][1];
      impares[j][2] = tri[i][2];
   }

   CalcularVectoresNormales();

   // Asignamos colores a los vértices según su normal
   for (unsigned i = 0; i < ver.size(); i++)
   {
      Tupla3f color(fabs(ver[i][X]),fabs(ver[i][Y]),fabs(ver[i][Z]));
      colores_vertices.push_back(color);
   }

   int
   tam_ver = sizeof(float)*3L*num_verts ,
   tam_tri = sizeof(int)*3L*num_tri ,
   tam_pares = sizeof(int)*3L*num_pares ,
   tam_impares = sizeof(int)*3L*num_impares ;

   this->modo = modo;

   vbo_vertices = new VBO(GL_ARRAY_BUFFER, tam_ver, ver[0].getPuntero());
   vbo_triangulos = new VBO(GL_ELEMENT_ARRAY_BUFFER,tam_tri, tri[0].getPuntero());
   vbo_pares = new VBO(GL_ELEMENT_ARRAY_BUFFER,tam_pares, pares[0].getPuntero());
   vbo_impares = new VBO(GL_ELEMENT_ARRAY_BUFFER,tam_impares, impares[0].getPuntero());
   vbo_colores_vertices = new VBO(GL_ARRAY_BUFFER, tam_ver, colores_vertices[0].getPuntero());
   vbo_normales_vertices = new VBO(GL_ARRAY_BUFFER, tam_ver, normales_vertices[0].getPuntero());

}

void MallaTVT::CalcularVectoresNormales()
{
   // Primero las normales de las caras y baricentros
   for (unsigned cara = 0; cara < tri.size(); cara++)
   {
      Tupla3f A = ver[tri[cara][X]];
      Tupla3f B = ver[tri[cara][Y]];
      Tupla3f C = ver[tri[cara][Z]];

      Tupla3f AB = B - A;
      Tupla3f BC = C - B;

      Tupla3f baricentro;
      baricentro[X] = (A[X] + B[X] + C[X])/3;
      baricentro[Y] = (A[Y] + B[Y] + C[Y])/3;
      baricentro[Z] = (A[Z] + B[Z] + C[Z])/3;
      baricentros.push_back(baricentro);

      normales_caras.push_back((AB*BC).normalized());
   }

   for (unsigned vertice = 0; vertice < ver.size(); vertice++)
   {
      normales_vertices.push_back(Tupla3f(0.0,0.0,0.0));
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
   }
}

void MallaTVT::Visualizar()
{

   glColor3f(0.0,0.0,1.0);
   switch (modo)
   {
      case ALAMBRE:
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         break;
      case SOLIDO:
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         break;
      case PUNTOS:
         glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
         break;
      case AJEDREZ:
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         break;
   }

   // Ver si usamos array de colores o vértices
   if (!colores_vertices.empty())
   {

      glBindBuffer( GL_ARRAY_BUFFER, vbo_colores_vertices->getID() );
      glColorPointer (3, GL_FLOAT, 0, 0);
      glEnableClientState( GL_COLOR_ARRAY );

   }

   if (!normales_vertices.empty())
   {
      glBindBuffer( GL_ARRAY_BUFFER, vbo_normales_vertices->getID() );
      glNormalPointer ( GL_FLOAT, 0, 0);
      glEnableClientState( GL_NORMAL_ARRAY );

   }


   // especificar formato de los vértices en su VBO (y offset)
   glBindBuffer( GL_ARRAY_BUFFER, vbo_vertices->getID() ); // act. VBO
   glVertexPointer( 3, GL_FLOAT, 0, 0 ); // formato y offset (0)
   glBindBuffer( GL_ARRAY_BUFFER, 0 ); // desact VBO.
   glEnableClientState( GL_VERTEX_ARRAY ); // act. uso VA

   unsigned num_tri = tri.size();
   unsigned num_pares = pares.size();
   unsigned num_impares = impares.size();

   // visualizar con glDrawElements (puntero a tabla == NULL)
   if (modo != AJEDREZ)
   {

      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo_triangulos->getID() );
      glDrawElements( GL_TRIANGLES, 3L*num_tri, GL_UNSIGNED_INT, NULL ) ;
   }
   else
   {
      glDisableClientState(GL_COLOR_ARRAY);
      glColor3f(0.0,0.0,0.0);
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo_pares->getID() );
      glDrawElements( GL_TRIANGLES, 3L*num_pares, GL_UNSIGNED_INT, NULL ) ;

      glColor3f(1.0,1.0,1.0);
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo_impares->getID() );
      glDrawElements( GL_TRIANGLES, 3L*num_impares, GL_UNSIGNED_INT, NULL ) ;
   }
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

   // desactivar uso de array de vértices
   glDisableClientState( GL_VERTEX_ARRAY );

   if (!colores_vertices.empty())
      glDisableClientState( GL_COLOR_ARRAY );
   if (!normales_vertices.empty())
         glDisableClientState( GL_NORMAL_ARRAY );
}

void MallaTVT::VisualizarNormalesCaras()
{
   glColor3f(0.0,0.0,0.0);
   for (unsigned cara = 0; cara < baricentros.size(); cara++)
   {
      glBegin(GL_LINES);

      glVertex3fv(baricentros[cara].getPuntero());
      Tupla3f extremo = baricentros[cara] + normales_caras[cara];
      glVertex3fv(extremo.getPuntero());

      glEnd();
   }
}

void MallaTVT::VisualizarNormalesVertices()
{
   glColor3f(0.0,0.0,0.0);
   for (unsigned vertice = 0; vertice < ver.size(); vertice++)
   {
      glBegin(GL_LINES);

      glVertex3fv(ver[vertice].getPuntero());
      Tupla3f extremo = ver[vertice] + normales_vertices[vertice];
      glVertex3fv(extremo.getPuntero());

      glEnd();
   }
}

MallaTVT* MallaTVT::Revolucion(unsigned caras)
{

   float alpha = 2*M_PI/caras;

   float rotacion[4][4] = {
         {cosf(alpha),0,sinf(alpha),0},
         {0,1,0,0},
         {(-1)*sinf(alpha),0,cosf(alpha),0},
         {0,0,0,1}
   };

   Matriz4x4 rot(rotacion);

   std::vector<Tupla3f> centro_tapas;


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
   std::vector<std::vector<Tupla3f> > perfiles;

   perfiles.push_back(ver);
   std::vector<Tupla3f> perfil_actual;
   std::vector<int> vertices_fijos;
   Tupla3f vertice_actual;

   for (unsigned perfil = 1; perfil < caras; perfil++)
   {
      perfil_actual.clear();
      for (unsigned i = 0; i < vertices_perfil; i++)
      {
         vertice_actual = rot*perfiles[perfil-1][i];
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
         unsigned indice_vertice_coplanario = indice_vertice_actual + vertices_perfil;
         unsigned indice_vertice_coplanario_anterior = indice_vertice_anterior + vertices_perfil;

         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_coplanario_anterior));
         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_coplanario_anterior, indice_vertice_coplanario));
      }
   }

   // Último perfil a fuego
   unsigned perfil = caras - 1;
   for (unsigned vertice = 1; vertice < vertices_perfil; vertice++)     // Cogemos los triángulos igual que en el guión de prácticas
   {
      unsigned indice_vertice_actual = perfil * vertices_perfil + vertice;
      unsigned indice_vertice_anterior = indice_vertice_actual - 1;
      unsigned indice_vertice_coplanario = vertice;
      unsigned indice_vertice_coplanario_anterior = vertice - 1;

      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_coplanario_anterior));
      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_coplanario, indice_vertice_coplanario_anterior));
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


   // Lo devolvemos a formato GLfloat e int y devolvemos un puntero a una malla nueva

   std::vector<GLfloat> v_res;

   for (unsigned i = 0; i < ver.size(); i++)
   {
      v_res.push_back(ver[i][0]);
      v_res.push_back(ver[i][1]);
      v_res.push_back(ver[i][2]);
   }

   std::vector<int> c_res;

   for (unsigned i = 0; i < tri.size(); i++)
   {
      c_res.push_back(tri[i][0]);
      c_res.push_back(tri[i][1]);
      c_res.push_back(tri[i][2]);
   }

   MallaTVT *res = new MallaTVT(v_res,ALAMBRE,c_res);

   delete this; // Cuidado! Después de esto NO TOCAR EL PROPIO OBJETO

   return res;

}

MallaTVT* MallaTVT::Barrido(unsigned caras)
{

   float alpha = 2*M_PI/caras;

   float rotacion[4][4] = {
         {cosf(alpha),0,sinf(alpha),0},
         {0,1,0,0},
         {(-1)*sinf(alpha),0,cosf(alpha),0},
         {0,0,0,1}
   };

   Matriz4x4 rot(rotacion);

   std::vector<Tupla3f> centro_tapas;

   unsigned vertices_perfil = ver.size();


   // Crear matriz de perfiles
   std::vector<std::vector<Tupla3f> > perfiles;

   perfiles.push_back(ver);
   std::vector<Tupla3f> perfil_actual;
   std::vector<int> vertices_fijos;
   Tupla3f vertice_actual;

   for (unsigned perfil = 1; perfil < caras; perfil++)
   {
      perfil_actual.clear();
      for (unsigned i = 0; i < vertices_perfil; i++)
      {
         vertice_actual = rot*perfiles[perfil-1][i];
         perfil_actual.push_back(vertice_actual);
         ver.push_back(vertice_actual);
      }
      perfiles.push_back(perfil_actual);
   }



   // Añadir triángulos

   unsigned indice_vertice_actual;
   unsigned indice_vertice_anterior;
   unsigned indice_vertice_coplanario;
   unsigned indice_vertice_coplanario_anterior;


   for (unsigned perfil = 0; perfil < caras-1; perfil++)
   {
      for (unsigned vertice = 1; vertice < vertices_perfil; vertice++)     // Cogemos los triángulos igual que en el guión de prácticas
      {
         indice_vertice_actual = perfil * vertices_perfil + vertice;
         indice_vertice_anterior = indice_vertice_actual - 1;
         indice_vertice_coplanario = indice_vertice_actual + vertices_perfil;
         indice_vertice_coplanario_anterior = indice_vertice_anterior + vertices_perfil;

         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_coplanario_anterior));
         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_coplanario, indice_vertice_coplanario_anterior));
      }

      indice_vertice_actual = perfil * vertices_perfil;
      indice_vertice_anterior = indice_vertice_actual + vertices_perfil - 1;
      indice_vertice_coplanario = indice_vertice_actual + vertices_perfil;
      indice_vertice_coplanario_anterior = indice_vertice_anterior + vertices_perfil;

      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_coplanario_anterior));
      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_coplanario, indice_vertice_coplanario_anterior));

   }


   // Último perfil a fuego
   unsigned perfil = caras - 1;
   for (unsigned vertice = 1; vertice < vertices_perfil; vertice++)     // Cogemos los triángulos igual que en el guión de prácticas
   {
      indice_vertice_actual = perfil * vertices_perfil + vertice;
      indice_vertice_anterior = indice_vertice_actual - 1;
      indice_vertice_coplanario = vertice;
      indice_vertice_coplanario_anterior = vertice - 1;

      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_coplanario_anterior));
      tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_coplanario, indice_vertice_coplanario_anterior));
   }


   // Último rectángulo a fuego
   // El actual es el primero del último perfil
   // El anterior es el último del último perfil
   // El coplanario es el 0
   // El coplanario anterior es el último del primer perfil
   indice_vertice_actual = perfil * vertices_perfil;
   indice_vertice_anterior = indice_vertice_actual + vertices_perfil - 1;
   indice_vertice_coplanario = 0;
   indice_vertice_coplanario_anterior = vertices_perfil - 1;

   tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_coplanario_anterior));
   tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_coplanario, indice_vertice_coplanario_anterior));





   // Lo devolvemos a formato GLfloat e int y devolvemos un puntero a una malla nueva

   std::vector<GLfloat> v_res;

   for (unsigned i = 0; i < ver.size(); i++)
   {
      v_res.push_back(ver[i][0]);
      v_res.push_back(ver[i][1]);
      v_res.push_back(ver[i][2]);
   }

   std::vector<int> c_res;

   for (unsigned i = 0; i < tri.size(); i++)
   {
      c_res.push_back(tri[i][0]);
      c_res.push_back(tri[i][1]);
      c_res.push_back(tri[i][2]);
   }

   MallaTVT *res = new MallaTVT(v_res,ALAMBRE,c_res);

   delete this; // Cuidado! Después de esto NO TOCAR EL PROPIO OBJETO

   return res;

}

enum visualizacion MallaTVT::getModo()
{
   return modo;
}

void MallaTVT::CambioModoDibujo(enum visualizacion modo)
{
   this->modo = modo;
}
