#include <GL/glew.h>
#include <GL/glut.h>
#include "VBO.hpp"

VBO::VBO( GLuint tipo, GLuint tamanio, GLvoid * puntero )
{
   if (!( tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER ))
      exit(-2);
   this->tipo = tipo;
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
