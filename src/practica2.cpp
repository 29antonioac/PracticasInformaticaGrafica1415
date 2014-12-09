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
   malla = NULL;
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


   malla = new MallaTVT(vertices_ply);


   if (M == 0)
      malla = malla->Revolucion(N);
   else if (M == 1)
      malla = malla->Barrido_Rotacion(N);
   else if (M == 2)
      malla = malla->Barrido_Traslacion(N,0.5,0.5,0.5);

   cout << "-----------------------------------" << endl;
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
   switch(toupper(tecla))
   {
      case 'L':
         CambioModoDibujo(ALAMBRE);
         break;
      case 'S':
         CambioModoDibujo(SOLIDO);
         break;
      case 'D':
         CambioModoDibujo(SOLIDO_CARAS);
         break;
      case 'P':
         CambioModoDibujo(PUNTOS);
         break;
      case 'A':
         CambioModoDibujo(AJEDREZ);
         break;
      case 'N':
         CambioModoNormales();
         break;
      case 'F':
         CambioColorFijo();
         break;
      default:
         redisp = false;
         break;
   }
   return redisp;
}

void Practica2::Debug()
{
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   gluOrtho2D(0.0, 1024, 0.0, 800);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glColor3f(1.0f, 0.0f, 0.0f);

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
   debug_strings.push_back(string("Modo de normales: " + enumToString(malla->getModoNormales())));
   debug_strings.push_back(string("Color fijo: " + str_color_fijo));
   debug_strings.push_back(string("Modo de dibujo: " + enumToString(malla->getModoDibujo())));
   debug_strings.push_back(string("Modo: " + str_modo));
   debug_strings.push_back(string("Numero de perfiles: " + std::to_string(N)));
   debug_strings.push_back(string("Perfil cargado: " + file));
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
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   //glEnable(GL_TEXTURE_2D);
}
