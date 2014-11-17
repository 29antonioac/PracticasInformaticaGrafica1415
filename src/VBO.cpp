#include "VBO.hpp"

VBO::VBO( GLuint tipo, GLuint numero_datos, GLuint tamanio, GLvoid * puntero )
{
   if (!( tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER ))
      exit(-2);
   this->tipo = tipo;
   this->numero_datos = numero_datos;
   this->tamanio = tamanio;
   this->datos = puntero;
   //GLuint id_vbo ; // resultado: identificador de VBO
   glGenBuffers( 1, & id_vbo ); // crear nuevo VBO, obtener identificador
   glBindBuffer( tipo, id_vbo ); // activar el VBO usando su identificador
   glBufferData( tipo, tamanio, puntero, GL_STATIC_DRAW );
   // transferencia RAM –> GPU
   glBindBuffer( tipo, 0 ); // desactivación del VBO (activar 0)
   //return id_vbo ; // devolver el identificador resultado
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

void VBO_Colores::Activar()
{
   glBindBuffer( tipo, id_vbo ); // act. VBO
   glColorPointer (3, GL_FLOAT, 0, 0);
   glBindBuffer( tipo, 0 ); // desact VBO.
   glEnableClientState( GL_COLOR_ARRAY );
}

void VBO_Normales::Activar()
{
   glBindBuffer( tipo, id_vbo ); // act. VBO
   glNormalPointer ( GL_FLOAT, 0, 0);
   glBindBuffer( tipo, 0 ); // desact VBO.
   glEnableClientState( GL_NORMAL_ARRAY );
}

void VBO_Triangulos::Visualizar(enum visualizacion modo)
{
   if (modo != AJEDREZ)
   {
      glBindBuffer(tipo, id_vbo);
      glDrawElements(GL_TRIANGLES, numero_datos, GL_UNSIGNED_INT, NULL) ;
      glBindBuffer(tipo, 0);
   }
   else
   {
      unsigned num_pares = numero_datos / 2;
      unsigned num_impares = numero_datos - num_pares;

      glDisableClientState(GL_COLOR_ARRAY);
      glColor3f(0.0,0.0,0.0);
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_vbo );
      glDrawElements( GL_TRIANGLES, num_impares, GL_UNSIGNED_INT, NULL ) ;

      glColor3f(1.0,1.0,1.0);
      glDrawElements( GL_TRIANGLES, num_pares, GL_UNSIGNED_INT, (const void *) (num_impares*sizeof(GLuint)) ) ;
      glBindBuffer(tipo,0);
   }
}









