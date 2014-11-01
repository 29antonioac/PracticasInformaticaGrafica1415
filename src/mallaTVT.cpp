#include "mallaTVT.hpp"



MallaTVT::MallaTVT (std::vector<GLfloat> vertices, std::vector<int> caras, enum visualizacion modo)
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

   //MTVT_Crear_VBOs();
   vbo_vertices = new VBO(GL_ARRAY_BUFFER, tam_ver, ver);
   vbo_triangulos = new VBO(GL_ELEMENT_ARRAY_BUFFER,tam_tri,tri);
   vbo_pares = new VBO(GL_ELEMENT_ARRAY_BUFFER,tam_pares,pares);
   vbo_impares = new VBO(GL_ELEMENT_ARRAY_BUFFER,tam_impares,impares);
   //VBO_Crear( GL_ARRAY_BUFFER,
     //       tam_ver, ver );
}

/*MallaTVT::MallaTVT (std::vector<GLfloat> vertices, enum visualizacion modo)
{
   num_verts = vertices.size()/3;
   //num_tri = caras.size()/3;

   //num_pares = num_tri / 2;
   //num_impares = num_tri - num_pares;

   ver = new Tupla3f[num_verts];
   //tri = new Tupla3i[num_tri];

   //pares = new Tupla3i[num_pares];
   //impares = new Tupla3i[num_impares];

   unsigned i,j;

   for ( i = 0, j = 0 ; j < num_verts ; i+=3, j++ )
   {
      ver[j].coo[0] = vertices.at(i);
      ver[j].coo[1] = vertices.at(i+1);
      ver[j].coo[2] = vertices.at(i+2);
   }

   //for ( i = 0, j = 0 ; j < num_tri ; i+=3, j++ )
   //{
   //   tri[j].idx[0] = caras.at(i);
   //   tri[j].idx[1] = caras.at(i+1);
   //   tri[j].idx[2] = caras.at(i+2);
   //}



//   for (i = 0, j = 0; i < num_tri; i+=2, j++)
//   {
//      pares[j].idx[0] = tri[i].idx[0];
//      pares[j].idx[1] = tri[i].idx[1];
//      pares[j].idx[2] = tri[i].idx[2];
//   }
//
//
//   for (i = 1, j = 0; i < num_tri; i+=2, j++)
//   {
//      impares[j].idx[0] = tri[i].idx[0];
//      impares[j].idx[1] = tri[i].idx[1];
//      impares[j].idx[2] = tri[i].idx[2];
//   }

   //tam_ver = sizeof(float)*3L*num_verts ;
   //tam_tri = sizeof(int)*3L*num_tri ;
   //tam_pares = sizeof(int)*3L*num_pares ;
   //tam_impares = sizeof(int)*3L*num_impares ;

   this->modo = modo;

   //MTVT_Crear_VBOs();
}*/

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
