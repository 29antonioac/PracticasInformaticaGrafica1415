#include "mallaTVT.hpp"



MallaTVT::MallaTVT(std::vector<GLfloat> vertices, enum visualizacion modo, std::vector<int> caras)
{
   num_verts = vertices.size()/3;
   num_tri = caras.size()/3;

   // Esta división tan rara de triángulos
   // pares e impares se hace para que en caso
   // de número total de triángulos impares no quede
   // ninguno suelto

   num_pares = num_tri / 2;
   num_impares = num_tri - num_pares;

   ver = new Tupla3f[num_verts];
   tri = new Tupla3i[num_tri];

   pares = new Tupla3i[num_pares];
   impares = new Tupla3i[num_impares];

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

   vbo_vertices = new VBO(GL_ARRAY_BUFFER, tam_ver, ver);
   vbo_triangulos = new VBO(GL_ELEMENT_ARRAY_BUFFER,tam_tri,tri);
   vbo_pares = new VBO(GL_ELEMENT_ARRAY_BUFFER,tam_pares,pares);
   vbo_impares = new VBO(GL_ELEMENT_ARRAY_BUFFER,tam_impares,impares);

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

   float rotacion[4][4] = {
         {cos(alpha),0,sin(alpha),0},
         {0,1,0,0},
         {-sin(alpha),0,cos(alpha),0},
         {0,0,0,1}
   };

   Matriz4x4 rot(rotacion);

   std::vector<Tupla3f> vertices_temp;
   std::vector<Tupla3i> caras_temp;

   // Añadimos todos los vértices a un vector para no explotar
   /*if (ver[0][X] != 0.0f || ver[0][Z] != 0.0f)
   {
      //std::cout << "Necesito tapa inferior " <<  ver[0] << std::endl;
      vertices_temp.push_back(Tupla3f(0.0,ver[0][Y],0.0));
   }*/

   for (unsigned i = 0; i < num_verts; i++)
   {
      vertices_temp.push_back(ver[i]);
   }

   /*if (ver[num_verts-1][X] != 0.0f || ver[num_verts-1][Z] != 0.0f)
   {
      //std::cout << "Necesito tapa superior " <<  ver[num_verts-1] << std::endl;
      vertices_temp.push_back(Tupla3f(0.0,ver[num_verts-1][Y],0.0));
   }*/

   /*for (unsigned i = 0; i < vertices_temp.size(); i++)
   {
      std::cout << vertices_temp[i] << std::endl;
   }*/

   unsigned vertice = 0;
   //num_verts = vertices_temp.size();
   std::cout << "Tengo " << num_verts << " vértices! " << std::endl;
   for (unsigned perfil = 1; perfil < caras; perfil++)
   {
      vertice = perfil*num_verts;
      for (unsigned i = 0; i < num_verts; i++, vertice++)
      {
         vertices_temp.push_back(rot*vertices_temp[vertice-num_verts]);
      }

      vertice = perfil*num_verts;
      for (unsigned i = vertice; i < vertice+num_verts - 1; i++)
      {
         Tupla3i cara1 = Tupla3i(i,i-num_verts+1,i-num_verts);
         Tupla3i cara2 = Tupla3i(i,i-num_verts+1,i+1);

         caras_temp.push_back(cara1);
         caras_temp.push_back(cara2);

      }
   }

   // La última cara la ponemos a fuego
   unsigned vertices_totales = vertices_temp.size();
   vertice = vertices_totales - num_verts + 1;

   for (unsigned i = vertice; i < vertice+num_verts - 1; i++)
   {
      Tupla3i cara1 = Tupla3i(i,i-vertice,i-vertice+1);
      Tupla3i cara2 = Tupla3i(i,i-vertice,i-1);

      caras_temp.push_back(cara2);
      caras_temp.push_back(cara1);

   }

//#define TAPA_SUP
#define TAPA_INF

#ifdef TAPA_INF

   // Agregamos las tapas

   // Tapa inferior
   if (ver[0][X] != 0.0f || ver[0][Z] != 0.0f)
   {
      vertices_temp.push_back(Tupla3f(0.0,ver[0][Y],0.0));
   }

   int centro_tapa_inferior = vertices_temp.size()-1;
   for (unsigned i = 0; i < caras*num_verts; i+=num_verts)
   {
      Tupla3i cara = Tupla3i(centro_tapa_inferior,i,i+num_verts);
      caras_temp.push_back(cara);
   }
   caras_temp.push_back(Tupla3i(centro_tapa_inferior,(caras-1)*num_verts,0));

#endif

#ifdef TAPA_SUP

   // Tapa superior
   if (ver[num_verts-1][X] != 0.0f || ver[num_verts-1][Z] != 0.0f)
   {
      vertices_temp.push_back(Tupla3f(0.0,ver[num_verts-1][Y],0.0));
   }

   int centro_tapa_superior = vertices_temp.size()-1;
   for (unsigned i = num_verts-1; i < caras*num_verts; i+=num_verts)
   {
      Tupla3i cara = Tupla3i(centro_tapa_superior,i,i+num_verts);
      caras_temp.push_back(cara);
   }

   //caras_temp.push_back(Tupla3i(centro_tapa_superior,caras*num_verts-1,num_verts-1));
   //caras_temp.push_back(Tupla3i(centro_tapa_inferior,(caras-1)*num_verts,0));

#endif


   // Lo devolvemos a formato GLfloat e int y devolvemos un puntero a una malla nueva

   std::vector<GLfloat> v_res;

   for (unsigned i = 0; i < vertices_temp.size(); i++)
   {
      v_res.push_back(vertices_temp[i][0]);
      v_res.push_back(vertices_temp[i][1]);
      v_res.push_back(vertices_temp[i][2]);
   }

   std::vector<int> c_res;

   for (unsigned i = 0; i < caras_temp.size(); i++)
   {
      c_res.push_back(caras_temp[i][0]);
      c_res.push_back(caras_temp[i][1]);
      c_res.push_back(caras_temp[i][2]);
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
