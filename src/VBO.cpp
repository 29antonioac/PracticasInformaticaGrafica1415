#include "VBO.hpp"

VBO::VBO( GLuint tipo, GLuint numero_datos, GLuint tamanio, GLvoid * puntero )
{
   if (!( tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER ))
   {
      exit(-2);
   }

   this->tipo = tipo;
   this->numero_datos = numero_datos;
   this->tamanio = tamanio;
   this->datos = puntero;

   glGenBuffers( 1, & id_vbo );
   glBindBuffer( tipo, id_vbo );
   glBufferData( tipo, tamanio, puntero, GL_STATIC_DRAW );

   glBindBuffer( tipo, 0 );
}

VBO::~VBO()
{
   glDeleteBuffers(1, &id_vbo);
}

GLuint VBO::getID()
{
   return id_vbo;
}

GLvoid * VBO::getDatos()
{
   return datos;
}

void VBO_Vertices::Activar()
{
   glBindBuffer( tipo, id_vbo ); // act. VBO
   glVertexPointer( 3, GL_FLOAT, 0, 0 ); // formato y offset (0)
   glBindBuffer( tipo, 0 ); // desact VBO.

   glEnableClientState( GL_VERTEX_ARRAY ); // act. uso VA
}

void VBO_Triangulos::Activar()
{}

void VBO_Lineas::Activar()
{

   glBindBuffer( tipo, id_vbo );
   glVertexPointer( 3, GL_FLOAT, 0, 0 );
   glBindBuffer( tipo, 0 );

   glEnableClientState( GL_VERTEX_ARRAY );

}

void VBO_Colores::Activar()
{
   glBindBuffer( tipo, id_vbo );
   glColorPointer (3, GL_FLOAT, 0, 0);
   glBindBuffer( tipo, 0 );

   glEnableClientState( GL_COLOR_ARRAY );
}

void VBO_Normales::Activar()
{
   glBindBuffer( tipo, id_vbo );
   glNormalPointer ( GL_FLOAT, 0, 0);
   glBindBuffer( tipo, 0 );

   glEnableClientState( GL_NORMAL_ARRAY );
}

void VBO_Vertices::Visualizar()
{
   glBindBuffer(tipo, id_vbo);
   glDrawArrays(GL_POINTS, 0, numero_datos);
   glBindBuffer(tipo, 0);

   glDisableClientState(GL_VERTEX_ARRAY);
}

void VBO_Triangulos::Visualizar(visualizacion modo, Tupla3f color_primario, Tupla3f color_secundario)
{
   if (modo != AJEDREZ)
   {
      glBindBuffer(tipo, id_vbo);
      glDrawElements(GL_TRIANGLES, numero_datos, GL_UNSIGNED_INT, 0) ;
      glBindBuffer(tipo, 0);
   }
   else
   {
      unsigned num_pares = numero_datos / 2;
      unsigned num_impares = numero_datos - num_pares;

      glDisableClientState(GL_COLOR_ARRAY);
      glColor3fv(color_primario.data());
      glBindBuffer( tipo, id_vbo );
      glDrawElements( GL_TRIANGLES, num_impares, GL_UNSIGNED_INT, 0 ) ;

      glColor3fv(color_secundario.data());
      glDrawElements( GL_TRIANGLES, num_pares, GL_UNSIGNED_INT, (const void *) (num_impares*sizeof(GLuint)) ) ;
      glBindBuffer(tipo,0);
   }

   glDisableClientState( GL_VERTEX_ARRAY );
}

void VBO_Lineas::Visualizar()
{
   glBindBuffer(tipo, id_vbo);
   glDrawArrays(GL_LINES, 0, numero_datos);
   glBindBuffer(tipo, 0);

   glDisableClientState(GL_VERTEX_ARRAY);
}

void VBO_Coordenadas_Textura::Activar()
{
   glBindBuffer(tipo, id_vbo);
   glTexCoordPointer(2,GL_FLOAT,0,0);
   glBindBuffer(tipo,0);

   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}









