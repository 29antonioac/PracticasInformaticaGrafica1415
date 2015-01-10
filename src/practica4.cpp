// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 4  (implementación)
// **
// *********************************************************************

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "practica4.hpp"

#include "MallaTVT.hpp"
#include "file_ply_stl.hpp"
#include "FuenteLuz.hpp"
#include "IDs_Shaders.hpp"

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::to_string;

template <class T>
inline T signo (T valor)
{
   if (valor > 0)
      return 1;
   else if (valor < 0)
      return -1;
   else
      return 0;
}

Practica4::Practica4()
{
   modo_dibujo = ALAMBRE;

   fuente_posicional = nullptr;
   fuente_direccional = nullptr;
   peon_madera = peon_blanco = peon_negro = cuerpo_lata = tapa_sup = tapa_inf = nullptr;
   raiz = nullptr;
   material_peon_madera = material_peon_blanco = material_peon_negro =
         material_cuerpo_lata = material_tapa = nullptr;

}

Practica4::~Practica4()
{

   //delete raiz;

   delete fuente_posicional;
   delete fuente_direccional;

   delete peon_madera; // Solo borramos el de madera ya que comparte vertices con los otros dos

   delete cuerpo_lata;
   delete tapa_sup;
   delete tapa_inf;

}

void Practica4::Inicializar( int argc, char *argv[] )
{
   unsigned caras_revolucion = 100;

   // Luces, cámara y acción!

   glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
   glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR ) ;
   glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );

   float alpha = 3 * M_PI / 4;
   float beta = M_PI / 6;
   glm::vec3 luz_posicional_componente_ambiental(0.0,0.0,1.0);
   glm::vec3 luz_posicional_componente_difusa(0.0,0.0,1.0);
   glm::vec3 luz_posicional_componente_especular(0.0,0.0,1.0);
   glm::vec3 luz_direccional_componente_ambiental(0.0,1.0,0.0);
   glm::vec3 luz_direccional_componente_difusa(0.0,1.0,0.0);
   glm::vec3 luz_direccional_componente_especular(0.0,1.0,0.0);
   glm::vec3 luz_posicion(0.0,10.0,0.0);

   fuente_direccional = new FuenteLuzDireccional(alpha, beta, luz_direccional_componente_ambiental, luz_direccional_componente_difusa, luz_direccional_componente_especular);
   fuente_posicional = new FuenteLuzPosicional(luz_posicion, luz_posicional_componente_ambiental, luz_posicional_componente_difusa, luz_posicional_componente_especular);
   fuentes.Agregar(fuente_direccional);
   fuentes.Agregar(fuente_posicional);

   // Cargar plys
   vector<GLfloat> vertices_ply_peon, vertices_ply_cuerpo_lata, vertices_ply_tapa_sup, vertices_ply_tapa_inf;
   ply::read_vertices("PLY/peon.ply", vertices_ply_peon);
   ply::read_vertices("PLY/lata-pcue.ply",vertices_ply_cuerpo_lata);
   ply::read_vertices("PLY/lata-psup.ply",vertices_ply_tapa_sup);
   ply::read_vertices("PLY/lata-pinf.ply",vertices_ply_tapa_inf);

   // Crear material del peon blanco (sin textura, material puramente difuso sin brillo especular)
   glm::vec3 peon_blanco_componente_emision(0.8,0.8,0.8);  // Color blanco
   glm::vec3 peon_blanco_componente_ambiental(0.0,0.0,0.0);
   glm::vec3 peon_blanco_componente_difusa(0.4,0.4,0.4);
   glm::vec3 peon_blanco_componente_especular(0.0,0.0,0.0);
   float peon_blanco_exponente_especular = 0.0;

   material_peon_blanco = new Material(peon_blanco_componente_emision, peon_blanco_componente_ambiental,
         peon_blanco_componente_difusa, peon_blanco_componente_especular, peon_blanco_exponente_especular);

   // Crear material del peón negro (sin textura, material especular sin apenas reflectividad difusa)
   glm::vec3 peon_negro_componente_emision(0.0,0.0,0.0);  // Color negro
   glm::vec3 peon_negro_componente_ambiental(0.0,0.0,0.0);
   glm::vec3 peon_negro_componente_difusa(0.1,0.1,0.1);
   glm::vec3 peon_negro_componente_especular(1.0,1.0,1.0);
   float peon_negro_exponente_especular = 30.0;

   material_peon_negro = new Material(peon_negro_componente_emision, peon_negro_componente_ambiental,
         peon_negro_componente_difusa, peon_negro_componente_especular, peon_negro_exponente_especular);

   // Crear material del peón de madera (con textura generada automáticamente, material difuso-especular)
   glm::vec3 peon_madera_componente_emision(1.0,1.0,1.0);  // Color blanco puro
   glm::vec3 peon_madera_componente_ambiental(0.0,0.0,0.0);
   glm::vec3 peon_madera_componente_difusa(0.1,0.1,0.1);
   glm::vec3 peon_madera_componente_especular(1.0,1.0,1.0);
   float peon_madera_exponente_especular = 30.0;

   float cs[4] = {1.0,0.0,0.0,0.0}; // Vector ex
   float ct[4] = {0.0,1.0,0.0,0.0}; // Vector ey

   Textura * textura_peon_madera = new Textura("img/text-madera.jpg",1,cs,ct);

   material_peon_madera = new Material(peon_madera_componente_emision, peon_madera_componente_ambiental,
         peon_madera_componente_difusa, peon_madera_componente_especular, peon_madera_exponente_especular,textura_peon_madera);

   // Crear peones

   peon_blanco = new MallaTVT(PERFIL,vertices_ply_peon);
   peon_blanco->Revolucion(caras_revolucion);
   peon_blanco->SetMaterial(material_peon_blanco);

   peon_negro = new MallaTVT(peon_blanco);
   //peon_negro = peon_negro->Revolucion(20);
   peon_negro->SetMaterial(material_peon_negro);

   peon_madera = new MallaTVT(peon_blanco);
   //peon_madera = peon_madera->Revolucion(20);
   peon_madera->SetMaterial(material_peon_madera);

   // Crear lata
   glm::vec3 cuerpo_lata_componente_emision(1.0,1.0,1.0);  // Color blanco puro
   glm::vec3 cuerpo_lata_componente_ambiental(0.0,0.0,0.0);
   glm::vec3 cuerpo_lata_componente_difusa(0.1,0.1,0.1);
   glm::vec3 cuerpo_lata_componente_especular(1.0,1.0,1.0);
   float cuerpo_lata_exponente_especular = 30.0;

   Textura * textura_cuerpo_lata = new Textura("img/text-lata-1.jpg",0,cs,ct);

   material_cuerpo_lata = new Material(cuerpo_lata_componente_emision, cuerpo_lata_componente_ambiental,
         cuerpo_lata_componente_difusa, cuerpo_lata_componente_especular, cuerpo_lata_exponente_especular,textura_cuerpo_lata);

   cuerpo_lata = new MallaTVT(PERFIL,vertices_ply_cuerpo_lata);
   cuerpo_lata->SetMaterial(material_cuerpo_lata);
   cuerpo_lata->Revolucion(caras_revolucion, false);  // Para calcular las coordenadas de textura hay que tener ya el material

   // Crear tapas
   glm::vec3 tapa_componente_emision(0.6,0.6,0.6);  // Color gris
   glm::vec3 tapa_componente_ambiental(0.0,0.0,0.0);
   glm::vec3 tapa_componente_difusa(0.0,0.0,0.0);
   glm::vec3 tapa_componente_especular(1.0,1.0,1.0);
   float tapa_exponente_especular = 15.0;

   material_tapa = new Material(tapa_componente_emision, tapa_componente_ambiental,
            tapa_componente_difusa, tapa_componente_especular, tapa_exponente_especular);

   tapa_sup = new MallaTVT(PERFIL,vertices_ply_tapa_sup);
   tapa_sup->SetMaterial(material_tapa);
   tapa_sup->Revolucion(caras_revolucion,false);
   tapa_inf = new MallaTVT(PERFIL,vertices_ply_tapa_inf);
   tapa_inf->SetMaterial(material_tapa);
   tapa_inf->Revolucion(caras_revolucion,false);




   raiz = new NodoGrafoEscena;
   glm::mat4 matriz_traslacion_peones(glm::translate(glm::mat4(1.0),glm::vec3(0.0,0.0,3.0)));
   glm::mat4 matriz_rotacion_peon_madera(glm::rotate(glm::mat4(1.0),(float)-M_PI/2,glm::vec3(0.0,1.0,0.0)));
   glm::mat4 matriz_rotacion_peon_negro(glm::rotate(glm::mat4(1.0),(float)M_PI/2,glm::vec3(0.0,1.0,0.0)));

   NodoGrafoEscena * nodo_traslacion_peon_madera = new NodoTransformacion(matriz_rotacion_peon_madera * matriz_traslacion_peones);
   NodoGrafoEscena * nodo_traslacion_peon_blanco = new NodoTransformacion(matriz_traslacion_peones);
   NodoGrafoEscena * nodo_traslacion_peon_negro = new NodoTransformacion(matriz_rotacion_peon_negro * matriz_traslacion_peones);
   NodoGrafoEscena * nodo_escalado_lata = new NodoTransformacion(glm::scale(glm::mat4(1.0),glm::vec3(4.0,4.0,4.0)));
   NodoGrafoEscena * nodo_escalado_tapas = new NodoTransformacion(glm::scale(glm::mat4(1.0),glm::vec3(4.0,4.0,4.0)));
   NodoGrafoEscena * nodo_peon_blanco = new NodoTerminal(peon_blanco);
   NodoGrafoEscena * nodo_peon_negro = new NodoTerminal(peon_negro);
   NodoGrafoEscena * nodo_peon_madera = new NodoTerminal(peon_madera);
   NodoGrafoEscena * nodo_cuerpo_lata = new NodoTerminal(cuerpo_lata);
   NodoGrafoEscena * nodo_tapa_sup = new NodoTerminal(tapa_sup);
   NodoGrafoEscena * nodo_tapa_inf = new NodoTerminal(tapa_inf);

   // Nodos Shader
   NodoGrafoEscena * nodo_shader_lata = new NodoShader(idProg_P4_lata);
   NodoGrafoEscena * nodo_shader_tapas = new NodoShader(idProg_P4_tapas);
   NodoGrafoEscena * nodo_shader_peon_blanco = new NodoShader(idProg_P4_peon_blanco);
   NodoGrafoEscena * nodo_shader_peon_madera = new NodoShader(idProg_P4_peon_madera);
   NodoGrafoEscena * nodo_shader_peon_negro = new NodoShader(idProg_P4_peon_negro);


   raiz->aniadeHijo(nodo_shader_peon_blanco);
      nodo_shader_peon_blanco->aniadeHijo(nodo_traslacion_peon_blanco);
         nodo_traslacion_peon_blanco->aniadeHijo(nodo_peon_blanco);

   raiz->aniadeHijo(nodo_shader_peon_negro);
      nodo_shader_peon_negro->aniadeHijo(nodo_traslacion_peon_negro);
         nodo_traslacion_peon_negro->aniadeHijo(nodo_peon_negro);

   raiz->aniadeHijo(nodo_shader_peon_madera);
      nodo_shader_peon_madera->aniadeHijo(nodo_traslacion_peon_madera);
         nodo_traslacion_peon_madera->aniadeHijo(nodo_peon_madera);

   raiz->aniadeHijo(nodo_shader_lata);
      nodo_shader_lata->aniadeHijo(nodo_escalado_lata);
         nodo_escalado_lata->aniadeHijo(nodo_cuerpo_lata);
   raiz->aniadeHijo(nodo_shader_tapas);
      nodo_shader_tapas->aniadeHijo(nodo_escalado_tapas);
         nodo_escalado_tapas->aniadeHijo(nodo_tapa_sup);
         nodo_escalado_tapas->aniadeHijo(nodo_tapa_inf);


}


void Practica4::DibujarObjetos()
{

   glEnable( GL_LIGHTING );
   glEnable( GL_NORMALIZE );
   glDisable( GL_COLOR_MATERIAL );

   // Dibujar aquí
   fuentes.Activar();
   raiz->Procesa();

   glDisable( GL_LIGHTING );
   glDisable( GL_NORMALIZE );
   glEnable( GL_COLOR_MATERIAL );

}

void Practica4::CambioModoDibujo(visualizacion modo_dibujo)
{
   //this->modo_dibujo = modo_dibujo;
}

void Practica4::CambioModoNormales()
{
   //this->cuerpo_lata->CambioModoNormales();

}


void Practica4::CambioGradoLibertad(int grado_libertad)
{

   if (grado_libertad != 1 && grado_libertad != -1
         && grado_libertad != 2 && grado_libertad != -2
         && grado_libertad != 3 && grado_libertad != -3
         && grado_libertad != 4 && grado_libertad != -4)
   {
      cout << "Grado de libertad inválido" << endl;
      exit(-4);
   }



}

void Practica4::CambioColorFijo()
{

}

bool Practica4::GestionarEvento(unsigned char tecla)
{
   bool redisp = true;

   switch(toupper(tecla))
   {
      // Control de luz direccional
      case 'Z':
         fuente_direccional->ModificaBeta(1);
         break;
      case 'X':
         fuente_direccional->ModificaBeta(-1);
         break;
      case 'C':
         fuente_direccional->ModificaAlpha(1);
         break;
      case 'V':
         fuente_direccional->ModificaAlpha(-1);
         break;
      default:
         redisp = false;
         break;
   }
   return redisp;
}


void Practica4::Debug()
{

   vector<string> debug_strings;

   string cauce;

   if (idProg_actual == 0)
      cauce = "Fijo";
   else
      cauce = "Programable";

   debug_strings.push_back(string("Beta: " + to_string(fuente_direccional->getBeta())));
   debug_strings.push_back(string("Alpha: " + to_string(fuente_direccional->getAlpha())));

   debug_strings.push_back(string("Modo de normales: " + enumToString(peon_madera->getModoNormales())));
   debug_strings.push_back(string("Modo de dibujo: " + enumToString(modo_dibujo)));
   debug_strings.push_back(string("Cauce: " + cauce));
   debug_strings.push_back(string("Practica 4"));

   void * font = GLUT_BITMAP_9_BY_15;

   unsigned num_lineas = 0;
   for (auto &s: debug_strings)
   {
      glRasterPos2i(10, 10+15*num_lineas);
      for (auto &c: s)
      {
        glutBitmapCharacter(font, c);
      }
      num_lineas++;
   }
}

void Practica4::Ayuda(vector<string> & strings_control)
{

   strings_control.push_back("Z/X para modificar beta (angulo con eje -X)");
   strings_control.push_back("C/V para modificar alpha (angulo con eje Y)");


}
