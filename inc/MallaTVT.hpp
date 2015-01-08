#ifndef __IG_MALLATVT_HPP__
#define __IG_MALLATVT_HPP__

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <utility>
#include <cmath>
#include <cstdlib>

#include "enumerados.hpp"
#include "error-ogl.hpp"
#include "VBO.hpp"
#include "Material.hpp"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr


using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::max;


class MallaTVT
{
private:

   vector<glm::vec3> ver;
   vector<glm::ivec3> tri;

   vector<glm::vec3> colores_vertices, normales_vertices, normales_caras, baricentros;
   vector<pair<glm::vec3,glm::vec3> > lineas_normales_caras, lineas_normales_vertices, colores_lineas_normales_caras, colores_lineas_normales_vertices;

   glm::vec3 color_primario;
   glm::vec3 color_secundario;

   VBO_Vertices   * vbo_vertices;
   VBO_Triangulos * vbo_triangulos;
   VBO_Colores    * vbo_colores_vertices;
   VBO_Colores    * vbo_colores_normales_caras;
   VBO_Colores    * vbo_colores_normales_vertices;
   VBO_Normales   * vbo_normales_vertices;
   VBO_Lineas     * vbo_lineas_normales_caras;
   VBO_Lineas     * vbo_lineas_normales_vertices;
   VBO_Coordenadas_Textura * vbo_coordenadas_textura;

   visualizacion modo_dibujo;
   normales dibujo_normales;
   tipo_malla tipo;

   Material * material;
   vector<pair<float,float> > coordenadas_textura;

   float dimension;

   bool color_fijo;

   GLuint id_VAO_malla, id_VAO_lineas[2];

   void CalcularVectoresNormales();
   void CalcularDimension();
   void CalcularCoordenadasTextura(unsigned vertices_perfil);
   void CrearVBOs();
   void Inicializar();
   void VisualizarModoInmediato();
   void VisualizarNormalesCaras();
   void VisualizarNormalesVertices();



public:
   MallaTVT(tipo_malla tipo, vector<GLfloat> vertices, vector<int> caras = vector<int>(), Material * material = nullptr);
   MallaTVT(tipo_malla tipo, vector<glm::vec3> vertices, vector<glm::ivec3> caras = vector<glm::ivec3>(), Material * material = nullptr);
   MallaTVT(MallaTVT * malla);
   ~MallaTVT();
   void SetMaterial(Material * material);
   void Visualizar();
   void Revolucion(const unsigned caras, bool tapas = true);
   void Barrido_Rotacion(const unsigned caras);
   void Barrido_Traslacion(const unsigned caras, const float dx, const float dy, const float dz);
   visualizacion getModoDibujo();
   normales getModoNormales();
   void CambioModoDibujo ( visualizacion modo ) ;
   void CambioModoNormales();
   void CambioColorFijo();
   bool ColorFijo();
};

#endif
