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

   malla = nullptr;
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

   vector<float> vertices_ply;
   vector<int> caras_ply;
   ply::read("PLY/big_atc.ply",vertices_ply,caras_ply);

   malla = new MallaTVT(MALLA,vertices_ply,caras_ply);

   raiz = new NodoGrafoEscena;
   NodoGrafoEscena * nodo_malla = new NodoTerminal(malla);
   raiz->aniadeHijo(nodo_malla);

   for (auto &c: camaras)
      c = new Camara(PRIMERA_PERSONA);

   camaraActual = camaras[0];

}


void Practica5::DibujarObjetos()
{
   // Dibujar aquí

   raiz->Procesa();

}

void Practica5::CambioModoDibujo(visualizacion modo_dibujo)
{
   this->modo_dibujo = modo_dibujo;

   this->malla->CambioModoDibujo(this->modo_dibujo);

}

void Practica5::CambioModoNormales()
{
   this->malla->CambioModoNormales();

}

void Practica5::CambioColorFijo()
{

}

bool Practica5::GestionarEvento(unsigned char tecla)
{
   bool redisp = true;

   const float desplazamiento = 0.05;

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
         /*
      case 'p':
         malla->CambioModoDibujo(PUNTOS);
         break;
      case 'a':
         malla->CambioModoDibujo(AJEDREZ);
         break;
      case 'h':
         malla->CambioModoNormales();
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
   debug_strings.push_back(string("Modo de normales: " + enumToString(malla->getModoNormales())));
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
