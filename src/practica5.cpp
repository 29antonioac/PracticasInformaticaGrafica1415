// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// ** Práctica 5  (implementación)
// **
// *********************************************************************

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "practica5.hpp"

#include "MallaTVT.hpp"
#include "file_ply_stl.hpp"
#include "Matriz.hpp"
#include "FuenteLuz.hpp"

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

Practica5::Practica5()
{
   modo_dibujo = ALAMBRE;

   raiz = nullptr;
   moto = peon = donut = nullptr;
   camaraActual = nullptr;

   for (auto &c: camaras)
         c = nullptr;

}

Practica5::~Practica5()
{

   delete raiz;


}

void Practica5::Inicializar( int argc, char *argv[] )
{

   const unsigned caras = 20;

   vector<float> vertices_moto_ply, vertices_peon_ply, vertices_donut_ply;
   vector<int> caras_moto_ply;

   ply::read("PLY/big_atc.ply",vertices_moto_ply,caras_moto_ply);
   ply::read_vertices("PLY/peon.ply",vertices_peon_ply);
   ply::read_vertices("PLY/toro.ply",vertices_donut_ply);

   moto = new MallaTVT(MALLA,vertices_moto_ply,caras_moto_ply);
   peon = new MallaTVT(PERFIL,vertices_peon_ply);
   donut = new MallaTVT(PERFIL,vertices_donut_ply);

   peon->Revolucion(caras);
   donut->Barrido_Rotacion(caras);

   mallas.push_back(moto);
   mallas.push_back(peon);
   mallas.push_back(donut);

   color_moto = Tupla3ub(255,0,0);
   moto->SetColorPrimario(color_moto);

   color_peon = Tupla3ub(0,255,0);
   peon->SetColorPrimario(color_peon);

   color_donut = Tupla3ub(0,0,255);
   donut->SetColorPrimario(color_donut);


   raiz = new NodoGrafoEscena;
   NodoGrafoEscena * nodo_moto = new NodoTerminal(moto);
   NodoGrafoEscena * nodo_peon = new NodoTerminal(peon);
   NodoGrafoEscena * nodo_donut = new NodoTerminal(donut);

   NodoGrafoEscena * nodo_traslacion_moto = new NodoTransformacion(Matriz4x4::Traslacion(-2.0,0.0,0.0));
   NodoGrafoEscena * nodo_traslacion_peon = new NodoTransformacion(Matriz4x4::Traslacion(4.0,0.0,0.0));
   NodoGrafoEscena * nodo_traslacion_donut = new NodoTransformacion(Matriz4x4::Traslacion(0.0,0.0,-3.0));


   raiz->aniadeHijo(nodo_traslacion_moto);
      nodo_traslacion_moto->aniadeHijo(nodo_moto);

   raiz->aniadeHijo(nodo_traslacion_peon);
      nodo_traslacion_peon->aniadeHijo(nodo_peon);

   raiz->aniadeHijo(nodo_traslacion_donut);
      nodo_traslacion_donut->aniadeHijo(nodo_donut);



   camaras[0] = new Camara(PRIMERA_PERSONA);
   camaras[1] = new Camara(PRIMERA_PERSONA);
   camaras[2] = new Camara(PRIMERA_PERSONA);

   camaraActual = camaras[0];

}

void Practica5::EnciendeLuces()
{

}

void Practica5::ClickRaton(int x, int y)
{
   // Ponemos el color fijo a las mallas
   CambioColorFijo();

   // Limpiamos la pantalla
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Deshabilitamos degradado y dibujamos. Volvemos a activar
   glDisable(GL_DITHER);
   raiz->Procesa();
   glEnable(GL_DITHER);

   // Conseguir valor del píxel clickado
   GLint viewport[4];
   GLubyte pixel[3];

   glGetIntegerv(GL_VIEWPORT,viewport);
   glReadPixels(x,viewport[3]-y,1,1,
         GL_RGB,GL_UNSIGNED_BYTE,(void*)pixel);

   // Comparar con nuestros valores de colores para cada figura
   if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255)
      cout << "Nada" << endl;
   else if (pixel[0] == 255)
      cout << "Moto" << endl;
   else if (pixel[1] == 255)
      cout << "Peón" << endl;
   else if (pixel[2] == 255)
      cout << "Donut" << endl;
   else
      cout << "Nada" << endl;

   // Restablecemos colores de normales de vértices
   CambioColorFijo();
}

void Practica5::DibujarObjetos()
{
   // Dibujar aquí

   raiz->Procesa();

}

void Practica5::CambioModoDibujo(visualizacion modo_dibujo)
{
   this->modo_dibujo = modo_dibujo;

   for (auto &malla: mallas)
      malla->CambioModoDibujo(this->modo_dibujo);

}

void Practica5::CambioModoNormales()
{
   for (auto &malla: mallas)
      malla->CambioModoNormales();

}

void Practica5::CambioColorFijo()
{
   for (auto &malla: mallas)
      malla->CambioColorFijo();
}

bool Practica5::GestionarEvento(unsigned char tecla)
{
   bool redisp = true;

   const float desplazamiento = 0.1;

   switch(tecla)
   {
      case 'w':
         camaraActual->ModificarPosicionZ(+desplazamiento);
         break;
      case 'a':
         camaraActual->ModificarPosicionX(+desplazamiento);
         break;
      case 's':
         camaraActual->ModificarPosicionZ(-desplazamiento);
         break;
      case 'd':
         camaraActual->ModificarPosicionX(-desplazamiento);
         break;
      case 'r':
         camaraActual->ReiniciarPosicion();
         break;

      case 'h':
         CambioModoNormales();
         break;
      case 'u':
         CambioColorFijo();
         break;

      case '1':
         camaraActual = camaras[0];
         break;
      case '2':
         camaraActual = camaras[1];
         break;
      case '3':
         camaraActual = camaras[2];
         break;
         /*
      case 'p':
         moto->CambioModoDibujo(PUNTOS);
         break;
      case 'a':
         moto->CambioModoDibujo(AJEDREZ);
         break;
      case 'h':
         moto->CambioModoNormales();
         break;*/
      default:
         redisp = false;
         break;
   }
   return redisp;
}


void Practica5::Debug()
{

   vector<string> debug_strings;

   debug_strings.push_back(string("Posición punto atención: " + string(camaraActual->getPuntoAtencion())));
   debug_strings.push_back(string("Posición observador: " + string(camaraActual->getObservador())));
   debug_strings.push_back(string("Modo de normales: " + enumToString(moto->getModoNormales())));
   debug_strings.push_back(string("Modo de dibujo: " + enumToString(modo_dibujo)));
   debug_strings.push_back(string("Practica 5"));

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

void Practica5::Ayuda(vector<string> & strings_control)
{

   strings_control.push_back("W/A/S/D para mover \"personaje\"");
   //strings_control.push_back("C/V para modificar alpha (angulo con eje Y)");


}

void Practica5::FijarCamara()
{
   camaraActual->FijarCamara();
}
void Practica5::FijarProyeccion(float ventana_tam_x, float ventana_tam_y)
{
   camaraActual->FijarProyeccion(ventana_tam_x, ventana_tam_y);
}
void Practica5::ModificaEjeXCamara(float nuevo)
{
   camaraActual->ModificaEjeX(nuevo);
}
void Practica5::ModificaEjeYCamara(float nuevo)
{
   camaraActual->ModificaEjeY(nuevo);
}
void Practica5::ModificarEscala(int signo)
{
   camaraActual->ModificarEscala(signo);
}
