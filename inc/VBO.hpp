#ifndef __IG_VBO_HPP__
#define __IG_VBO_HPP__

#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "visualizacion.hpp"
#include "tuplas.hpp"

using std::cout;
using std::endl;


class VBO
{
protected:
   GLuint id_vbo;
   GLuint tipo;
   unsigned int numero_datos;
   unsigned int tamanio;
   GLvoid * datos;
public:
   VBO( GLuint tipo, GLuint numero_datos, GLuint tamanio, GLvoid * puntero );
   virtual ~VBO() {};
   GLuint getID();
   GLvoid * getDatos();

   virtual void Activar() = 0;
};

class VBO_Vertices : virtual public VBO
{
public:
   VBO_Vertices(GLuint numero_datos, GLuint tamanio, GLvoid * puntero) : VBO(GL_ARRAY_BUFFER, numero_datos, tamanio, puntero) {}

   void Activar();
   void Visualizar();
};

class VBO_Colores : virtual public VBO
{
public:
   VBO_Colores(GLuint numero_datos, GLuint tamanio, GLvoid * puntero) : VBO(GL_ARRAY_BUFFER, numero_datos, tamanio, puntero) {}

   void Activar();
};

class VBO_Normales : virtual public VBO
{
public:
   VBO_Normales(GLuint numero_datos, GLuint tamanio, GLvoid * puntero) : VBO(GL_ARRAY_BUFFER, numero_datos, tamanio, puntero) {}

   void Activar();
};

class VBO_Triangulos : virtual public VBO
{
public:
   VBO_Triangulos(GLuint numero_datos, GLuint tamanio, GLvoid * puntero) : VBO(GL_ELEMENT_ARRAY_BUFFER, numero_datos, tamanio,  puntero) {}

   void Activar();
   void Visualizar(enum visualizacion modo, Tupla3f color_primario, Tupla3f color_secundario);
};

class VBO_Lineas : virtual public VBO
{
public:
   VBO_Lineas(GLuint numero_datos, GLuint tamanio, GLvoid * puntero) : VBO(GL_ARRAY_BUFFER, numero_datos, tamanio,  puntero) {}

   void Activar();
   void Visualizar();
};





#endif
