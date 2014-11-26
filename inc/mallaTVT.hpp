#ifndef __IG_MALLATVT_HPP__
#define __IG_MALLATVT_HPP__

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <utility>
#include <cmath>
#include <cstdlib>
#include "tuplas.hpp"
#include "error-ogl.hpp"
#include "visualizacion.hpp"
#include "VBO.hpp"
#include "Matriz.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::pair;


class MallaTVT
{
private:

   vector<Tupla3f> ver;
   vector<Tupla3i> tri;

   vector<Tupla3f> colores_vertices, normales_vertices, normales_caras, baricentros;
   vector<pair<Tupla3f,Tupla3f> > lineas_normales_caras, lineas_normales_vertices;

   Tupla3f color_primario;
   Tupla3f color_secundario;

   VBO_Vertices   * vbo_vertices;
   VBO_Triangulos * vbo_triangulos;
   VBO_Colores    * vbo_colores_vertices;
   VBO_Normales   * vbo_normales_vertices;
   VBO_Lineas     * vbo_lineas_normales_caras;
   VBO_Lineas     * vbo_lineas_normales_vertices;

   visualizacion modo_dibujo;
   normales dibujo_normales;

   float dimension;

   void CalcularVectoresNormales();
   void CalcularDimension();
   void CrearVBOs();
   void Inicializar();
   void VisualizarModoInmediato();
   void VisualizarNormalesCaras();
   void VisualizarNormalesVertices();

   MallaTVT(vector<Tupla3f> vertices, vector<Tupla3i> caras);

public:
   MallaTVT(vector<GLfloat> vertices, vector<int> caras = vector<int>() );
   void Visualizar();
   MallaTVT* Revolucion(const unsigned caras);
   MallaTVT* Barrido_Rotacion(const unsigned caras);
   MallaTVT* Barrido_Traslacion(const unsigned caras, const float dx, const float dy, const float dz);
   enum visualizacion getModo();
   void CambioModoDibujo ( enum visualizacion modo ) ;
   void CambioModoNormales();
};

#endif
