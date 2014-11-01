
#ifndef __IG_MALLATVT_HPP__
#define __IG_MALLATVT_HPP__

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include "tuplas2.hpp"
#include "error-ogl.hpp"
#include "visualizacion.hpp"
#include "VBO.hpp"


class MallaTVT
{
private:
   unsigned num_verts;
   unsigned num_tri;
   Tupla3f * ver;
   Tupla3i * tri;

   Tupla3i * pares;
   Tupla3i * impares;

   unsigned num_pares;
   unsigned num_impares;

   VBO * vbo_vertices;
   VBO * vbo_triangulos;
   VBO * vbo_pares;
   VBO * vbo_impares;

   visualizacion modo;

   //GLuint VBO_Crear( GLuint tipo, GLuint tamanio, GLvoid * puntero );
   //void MTVT_Crear_VBOs();

public:
   MallaTVT(std::vector<GLfloat> vertices, std::vector<int> caras, enum visualizacion modo);
   //MallaTVT(std::vector<GLfloat> vertices,enum visualizacion modo);
   void MTVT_Visualizar();
   enum visualizacion getModo();
   void CambioModoDibujo ( enum visualizacion modo ) ;

};

extern MallaTVT * pm_P1;
extern MallaTVT * pm_P2;


#endif
