// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include <cstdlib>
#include <iostream>
#include <string>
#include "practica2.hpp"
#include "file_ply_stl.hpp"
#include "Matriz.hpp"

using std::cout;
using std::vector;
using std::string;

Practica2::Practica2()
{
   malla = nullptr;
   camara = nullptr;
}

Practica2::~Practica2()
{
   delete malla;
}

void Practica2::Inicializar(int argc, char *argv[])
{


   if (argc == 1)
   {
      file = "PLY/arboldenavidad.ply";
      N = 20;
      M = 0;
   }
   else if (argc == 5)
   {
      file = string(argv[2]);
      N = atoi(argv[3]);
      M = atoi(argv[4]);

      if (N < 3)
      {
         cout << "Por favor, introduce N >= 3" << endl;
         exit(-1);
      }
      else if (N >= 10000)
      {
         cout << "N muy grande, puede tardar un poco en procesar..." << endl;
      }
   }
   else
   {
      cout << "Argumentos mal (argc = " << argc << ")" << endl;
      exit(-1);
   }

   cout << "Archivo: " << file << endl;

   ply::read_vertices(file.c_str(),vertices_ply);


   malla = new MallaTVT(PERFIL,vertices_ply);


   if (M == 0)
      malla->Revolucion(N);
   else if (M == 1)
      malla->Barrido_Rotacion(N);
   else if (M == 2)
      malla->Barrido_Traslacion(N,0.5,0.5,0.5);

   cout << "-----------------------------------" << endl;

   // Cámara
   camara = new Camara;
}

void Practica2::DibujarObjetos()
{
   malla->Visualizar();
}

void Practica2::CambioModoDibujo(visualizacion modo_dibujo)
{
   malla->CambioModoDibujo(modo_dibujo);
}

void Practica2::CambioModoNormales()
{
   malla->CambioModoNormales();
}

void Practica2::CambioColorFijo()
{
   malla->CambioColorFijo();
}

bool Practica2::GestionarEvento(unsigned char tecla)
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

void Practica2::Debug()
{

   string str_color_fijo;
   if (malla->ColorFijo())
      str_color_fijo = "Si";
   else
      str_color_fijo = "No";

   string str_modo;
   if (M == 0)
      str_modo = "Revolucion";
   else if (M == 1)
      str_modo = "Barrido rotacion";
   else if (M == 2)
      str_modo = "Barrido traslacion";

   vector<string> debug_strings;

   debug_strings.push_back(string("Modo: " + str_modo));
   debug_strings.push_back(string("Numero de perfiles: " + std::to_string(N)));
   debug_strings.push_back(string("Modo de normales: " + enumToString(malla->getModoNormales())));
   debug_strings.push_back(string("Color fijo: " + str_color_fijo));
   debug_strings.push_back(string("Modo de dibujo: " + enumToString(malla->getModoDibujo())));
   debug_strings.push_back(string("Archivo cargado: " + file));
   debug_strings.push_back(string("Practica 2"));
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

void Practica2::Ayuda(vector<string> & strings_control)
{

}

void Practica2::FijarCamara()
{
   camara->FijarCamara();
}
void Practica2::FijarProyeccion(float ventana_tam_x, float ventana_tam_y)
{
   camara->FijarProyeccion(ventana_tam_x, ventana_tam_y);
}
void Practica2::ModificaEjeXCamara(float nuevo)
{
   camara->ModificaEjeX(nuevo);
}
void Practica2::ModificaEjeYCamara(float nuevo)
{
   camara->ModificaEjeY(nuevo);
}
void Practica2::ModificarEscala(int signo)
{
   camara->ModificarEscala(signo);
}
