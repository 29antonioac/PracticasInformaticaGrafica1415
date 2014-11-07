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

   //ver = new Tupla3f[num_verts];
   //tri = new Tupla3i[num_tri];
   ver.resize(num_verts);
   tri.resize(num_tri);
   pares.resize(num_pares);
   impares.resize(num_impares);

   //pares = new Tupla3i[num_pares];
   //impares = new Tupla3i[num_impares];

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

   for (i = 0, j = 0; i < num_tri; i+=2, j++)
   {
      pares[j][0] = tri[i][0];
      pares[j][1] = tri[i][1];
      pares[j][2] = tri[i][2];
   }


   for (i = 1, j = 0; i < num_tri; i+=2, j++)
   {
      impares[j][0] = tri[i][0];
      impares[j][1] = tri[i][1];
      impares[j][2] = tri[i][2];
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

}

void MallaTVT::MTVT_Visualizar()
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
}

MallaTVT* MallaTVT::MTVT_Revolucion(unsigned caras)
{


   float alpha = 2*M_PI/caras;

   //caras = 4;

   /* Estoy a las agujas del reloj! Hay que hacerlo contrario! */



   float rotacion[4][4] = {
         {cos(alpha),0,sin(alpha),0},
         {0,1,0,0},
         {-sin(alpha),0,cos(alpha),0},
         {0,0,0,1}
   };

   Matriz4x4 rot(rotacion);

   std::vector<Tupla3f> centro_tapas;


   if (ver.front()[X] != 0.0f)
   {
      std::cout << "No hay tapa inferior!" << ver.front() << std::endl;
      centro_tapas.push_back(Tupla3f(0.0,ver.front()[Y],0.0));
   }
   else
   {
      std::cout << "Hay tapa inferior!" << ver.front() << std::endl;
      centro_tapas.push_back(ver.front());
      ver.erase(ver.begin());
   }

   if (ver.back()[X] != 0.0f)
   {
      std::cout << "No hay tapa superior!" << ver.back() << std::endl;
      centro_tapas.push_back(Tupla3f(0.0,ver.back()[Y],0.0));
   }
   else
   {
      std::cout << "Hay tapa superior!" << ver.back() << std::endl;
      centro_tapas.push_back(ver.back());
      ver.pop_back();
   }

   unsigned vertices_perfil = ver.size();

   std::cout << "Tengo " << vertices_perfil << " vértices! " << std::endl;

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


   for (unsigned perfil = 0; perfil < caras-1; perfil++)
   {
      for (unsigned vertice = 1; vertice < vertices_perfil; vertice++)     // Cogemos los triángulos igual que en el guión de prácticas
      {
         unsigned indice_vertice_actual = perfil * vertices_perfil + vertice;
         unsigned indice_vertice_anterior = indice_vertice_actual - 1;
         unsigned indice_vertice_coplanario = indice_vertice_actual + vertices_perfil;
         unsigned indice_vertice_coplanario_anterior = indice_vertice_anterior + vertices_perfil;

         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_anterior, indice_vertice_coplanario_anterior));
         tri.push_back(Tupla3i(indice_vertice_actual, indice_vertice_coplanario, indice_vertice_coplanario_anterior));
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

      Tupla3i triangulo(centro_tapa_inferior,vertice_actual,vertice_siguiente);
      tri.push_back(triangulo);
   }
   tri.push_back(Tupla3i(centro_tapa_inferior,(caras-1)*vertices_perfil,0));

   // Tapa superior
   ver.push_back(centro_tapas.front());
   centro_tapas.erase(centro_tapas.begin());

   int centro_tapa_superior = ver.size() - 1;
   for (unsigned cara = 0; cara < caras - 1; cara++)
   {
      unsigned vertice_actual = (cara + 1) * vertices_perfil - 1;
      unsigned vertice_siguiente = vertice_actual + vertices_perfil;

      Tupla3i triangulo(centro_tapa_superior,vertice_actual,vertice_siguiente);
      tri.push_back(triangulo);
   }
   //tri.push_back(Tupla3i(centro_tapa_superior, caras*vertices_perfil,vertices_perfil - 1));



//   for (unsigned perfil = 1; perfil < caras; perfil++)
//   {
//      vertice = perfil*num_verts;
//      for (unsigned i = 0; i < num_verts; i++, vertice++)
//      {
//         ver.push_back(rot*ver[vertice-num_verts]);
//      }
//
//      vertice = perfil*num_verts;
//      for (unsigned i = vertice; i < vertice+num_verts - 1; i++)
//      {
//         Tupla3i cara1 = Tupla3i(i,i-num_verts+1,i-num_verts);
//         Tupla3i cara2 = Tupla3i(i,i-num_verts+1,i+1);
//
//         tri.push_back(cara1);
//         tri.push_back(cara2);
//
//      }
//   }

   // La última cara la ponemos a fuego
   /*unsigned vertices_totales = ver.size();
   vertice = vertices_totales - num_verts + 1;

   for (unsigned i = vertice; i < vertice+num_verts - 1; i++)
   {
      Tupla3i cara1 = Tupla3i(i,i-vertice,i-vertice+1);
      Tupla3i cara2 = Tupla3i(i,i-vertice,i-1);

      tri.push_back(cara2);
      tri.push_back(cara1);

   }*/

   /*
//#define TAPA_INF
#define TAPA_SUP


#ifdef TAPA_INF

   // Agregamos las tapas

   // Tapa inferior
   if (necesito_tapa_inferior)
   {
      std::cout << "Necesito tapa inferior!" << std::endl;
      ver.push_back(Tupla3f(0.0,ver[0][Y],0.0));


      int centro_tapa_inferior = ver.size()-1;
      for (unsigned i = 0; i < caras*num_verts; i+=num_verts)
      {
         Tupla3i cara(centro_tapa_inferior,i,i+num_verts);
         tri.push_back(cara);
      }
      tri.push_back(Tupla3i(centro_tapa_inferior,(caras-1)*num_verts,0));
   }

#endif

#ifdef TAPA_SUP

   // Tapa superior
   if (necesito_tapa_superior)
   {
      std::cout << "Necesito tapa superior!" << std::endl;
      ver.push_back(Tupla3f(0.0,ver[num_verts-1][Y],0.0));


      int centro_tapa_superior = ver.size()-1;
      for (unsigned i = num_verts-1; i < caras*num_verts; i+=num_verts)
      {
         Tupla3i cara(centro_tapa_superior,i,i+num_verts);
         tri.push_back(cara);
      }

      //tri.push_back(Tupla3i(centro_tapa_superior,caras*num_verts-1,num_verts-1));
      //caras_temp.push_back(Tupla3i(centro_tapa_inferior,(caras-1)*num_verts,0));
   }

#endif*/


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

MallaTVT * pm_P1;
MallaTVT * pm_P2;
