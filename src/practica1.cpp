// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 1  (implementación)
// **
// *********************************************************************

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "practica1.hpp"
#include "file_ply_stl.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

Practica1::Practica1()
{
   malla = nullptr;
   camara = nullptr;
}

Practica1::~Practica1()
{
   delete malla;
}

void Practica1::Inicializar( int argc, char *argv[] )
{
   if (argc == 1)
   {
      file = "./PLY/ant.ply";
   }
   else if (argc == 5)
   {
      file = string(argv[1]);
   }
   else
   {
      cout << "Argumentos mal (argc = " << argc << ")" << endl;
      exit(-1);
   }

   cout << "Archivo: " << file << endl;

   ply::read(file.c_str(),vertices_ply,caras_ply);
   malla = new MallaTVT(MALLA,vertices_ply,caras_ply);

   cout << "-----------------------------------" << endl;

   // Cámara
   camara = new Camara;


   
}

// ---------------------------------------------------------------------
// función a implementar en la práctica 1  dibujar los objetos

void Practica1::DibujarObjetos()
{
   malla->Visualizar();
}

void Practica1::CambioModoDibujo(visualizacion modo_dibujo)
{
   malla->CambioModoDibujo(modo_dibujo);
}

void Practica1::CambioModoNormales()
{
   malla->CambioModoNormales();
}

void Practica1::CambioColorFijo()
{
   malla->CambioColorFijo();
}

bool Practica1::GestionarEvento(unsigned char tecla)
{
   bool redisp = true;
   switch(tecla)
   {
      case 'r':
         camara->ModificaEjeX(0.0);
         camara->ModificaEjeY(0.0);
         break;
      case 'l':
         malla->CambioModoDibujo(ALAMBRE);
         break;
      case 's':
         malla->CambioModoDibujo(SOLIDO);
         break;
      case 'd':
         malla->CambioModoDibujo(SOLIDO_CARAS);
         break;
      case 'p':
         malla->CambioModoDibujo(PUNTOS);
         break;
      case 'a':
         malla->CambioModoDibujo(AJEDREZ);
         break;
      case 'h':
         malla->CambioModoNormales();
         break;
   // Dejamos implementado el switch por si añadimos alguna tecla a esta práctica en particular
      default:
         redisp = false;
         break;
   }
   return redisp;
}

void Practica1::Debug()
{

   string str_color_fijo;
   if (malla->ColorFijo())
      str_color_fijo = "Si";
   else
      str_color_fijo = "No";

   vector<string> debug_strings;
   debug_strings.push_back(string("Rotacion eje Y: " + std::to_string(camara->getEjeY())));
   debug_strings.push_back(string("Rotacion eje X: " + std::to_string(camara->getEjeX())));
   debug_strings.push_back(string("Posición punto atención: " + string(camara->getPuntoAtencion())));
   debug_strings.push_back(string("Posición observador: " + string(camara->getObservador())));
   debug_strings.push_back(string("Modo de normales: " + enumToString(malla->getModoNormales())));
   debug_strings.push_back(string("Color fijo: " + str_color_fijo));
   debug_strings.push_back(string("Modo de dibujo: " + enumToString(malla->getModoDibujo())));
   debug_strings.push_back(string("Archivo cargado: " + file));
   debug_strings.push_back(string("Practica 1"));
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

void Practica1::Ayuda(vector<string> & strings_control)
{

}

void Practica1::FijarCamara()
{
   camara->FijarCamara();
}
void Practica1::FijarProyeccion(float ventana_tam_x, float ventana_tam_y)
{
   camara->FijarProyeccion(ventana_tam_x, ventana_tam_y);
}

void Practica1::ModificaEjeXCamara(float nuevo)
{
   camara->ModificaEjeX(nuevo);
}
void Practica1::ModificaEjeYCamara(float nuevo)
{
   camara->ModificaEjeY(nuevo);
}

void Practica1::ModificarEscala(int signo)
{
   camara->ModificarEscala(signo);
}
