#ifndef __IG_MALLATVT_HPP__
#define __IG_MALLATVT_HPP__

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <utility>
#include <cmath>
#include <cstdlib>

#include "enumerados.hpp"
#include "tuplas.hpp"
#include "error-ogl.hpp"
#include "VBO.hpp"
#include "Matriz.hpp"
#include "Material.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::max;


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
   VBO_Coordenadas_Textura * vbo_coordenadas_textura;

   visualizacion modo_dibujo;
   normales dibujo_normales;

   Material * material;
   vector<pair<Tupla3f,Tupla3f> > coordenadas_textura;

   float dimension;

   bool color_fijo;

   void CalcularVectoresNormales();
   void CalcularDimension();
   void CrearVBOs();
   void Inicializar();
   void VisualizarModoInmediato();
   void VisualizarNormalesCaras();
   void VisualizarNormalesVertices();



public:
   MallaTVT(vector<GLfloat> vertices, vector<int> caras = vector<int>(), Material * material = nullptr);
   MallaTVT(vector<Tupla3f> vertices, vector<Tupla3i> caras = vector<Tupla3i>(), Material * material = nullptr);
   MallaTVT(MallaTVT * malla);
   void SetMaterial(Material * material);
   void Visualizar();
   MallaTVT* Revolucion(const unsigned caras);
   MallaTVT* Barrido_Rotacion(const unsigned caras);
   MallaTVT* Barrido_Traslacion(const unsigned caras, const float dx, const float dy, const float dz);
   visualizacion getModoDibujo();
   normales getModoNormales();
   void CambioModoDibujo ( visualizacion modo ) ;
   void CambioModoNormales();
   void CambioColorFijo();
   bool ColorFijo();
};

#endif
