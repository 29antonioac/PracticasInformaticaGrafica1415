
#ifndef __IG_MALLATVT_HPP__
#define __IG_MALLATVT_HPP__

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <cmath>
#include "tuplas.hpp"
#include "error-ogl.hpp"
#include "visualizacion.hpp"
#include "VBO.hpp"
#include "Matriz.hpp"


class MallaTVT
{
private:

   std::vector<Tupla3f> ver;
   std::vector<Tupla3i> tri;

   std::vector<Tupla3f> colores_vertices, normales_vertices, normales_caras, baricentros;

   VBO_Vertices   * vbo_vertices;
   VBO_Triangulos * vbo_triangulos;
   VBO_Colores    * vbo_colores_vertices;
   VBO_Normales   * vbo_normales_vertices;

   visualizacion modo_dibujo;
   normales dibujo_normales;

   void CalcularVectoresNormales();
   void VisualizarModoInmediato();
   void VisualizarNormalesCaras();
   void VisualizarNormalesVertices();

   MallaTVT(std::vector<Tupla3f> vertices, std::vector<Tupla3i> caras);

public:
   MallaTVT(std::vector<GLfloat> vertices, std::vector<int> caras = std::vector<int>() );
   void Visualizar();
   MallaTVT* Revolucion(const unsigned caras);
   MallaTVT* Barrido_Rotacion(const unsigned caras);
   MallaTVT* Barrido_Traslacion(const unsigned caras, const float dx, const float dy, const float dz);
   enum visualizacion getModo();
   void CambioModoDibujo ( enum visualizacion modo ) ;
   void CambioModoNormales();

};

#endif
