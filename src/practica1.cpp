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
   malla = NULL;
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
   malla = new MallaTVT(vertices_ply,caras_ply);

   cout << "-----------------------------------" << endl;


   
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

void Practica1::Debug()
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

   vector<string> debug_strings;
   debug_strings.push_back(string("Modo de normales: " + enumToString(malla->getModoNormales())));
   debug_strings.push_back(string("Color fijo: " + str_color_fijo));
   debug_strings.push_back(string("Modo de dibujo: " + enumToString(malla->getModoDibujo())));
   debug_strings.push_back(string("Perfil cargado: " + file));
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
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   //glEnable(GL_TEXTURE_2D);
}
