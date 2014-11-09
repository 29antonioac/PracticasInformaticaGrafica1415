
#ifndef __IG_MALLATVT_HPP__
#define __IG_MALLATVT_HPP__

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <cmath>
#include "tuplas2.hpp"
#include "error-ogl.hpp"
#include "visualizacion.hpp"
#include "VBO.hpp"
#include "Matriz.hpp"


class MallaTVT
{
private:

   std::vector<Tupla3f> ver;
   std::vector<Tupla3i> tri,pares,impares;

   std::vector<Tupla3f> colores_vertices, normales_vertices, normales_caras, baricentros;

   VBO * vbo_vertices;
   VBO * vbo_triangulos;
   VBO * vbo_pares;
   VBO * vbo_impares;
   VBO * vbo_colores_vertices;
   VBO * vbo_normales_vertices;

   visualizacion modo;

   void CalcularVectoresNormales();

public:
   MallaTVT(std::vector<GLfloat> vertices, enum visualizacion modo, std::vector<int> caras = std::vector<int>() );
   //MallaTVT(std::vector<GLfloat> vertices, unsigned caras_longitudinales, enum visualizacion modo);
   void Visualizar();
   void VisualizarNormalesCaras();
   void VisualizarNormalesVertices();
   MallaTVT* Revolucion(unsigned caras);
   MallaTVT* Barrido(unsigned caras);
   enum visualizacion getModo();
   void CambioModoDibujo ( enum visualizacion modo ) ;

};

#endif
