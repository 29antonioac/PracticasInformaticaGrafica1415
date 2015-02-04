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
#include "problemas.hpp"
#include "file_ply_stl.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

Problemas::Problemas()
{
   malla = nullptr;
   camara = nullptr;
}

Problemas::~Problemas()
{
   delete malla;
}

void Problemas::Inicializar( int argc, char *argv[] )
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

void Problemas::EnciendeLuces()
{

}

void Problemas::ClickRaton(int x, int y)
{

}

// ---------------------------------------------------------------------
// función a implementar en la práctica 1  dibujar los objetos

// Funciones auxiliares
void gancho()
{
   glBegin(GL_LINE_STRIP);
      glVertex2f(0.0,2.0);
      glVertex2f(0.0,1.0);
      glVertex2f(1.0,1.0);
      glVertex2f(1.0,0.0);
      glVertex2f(0.0,0.0);
   glEnd();
}

void gancho_x4()
{
   glMatrixMode(GL_MODELVIEW);
   unsigned num_ganchos = 1;
   for (unsigned i = 0; i < num_ganchos; i++)
   {
      glTranslatef(-1.0,+1.0,0.0);
      glRotatef(90.0,0.0,0.0,1.0);
      glTranslatef(+1.0,-1.0,0.0);
      gancho();
   }
}


void figura_simple()
{
   glColor3f(0.0,0.0,0.8);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_QUADS);
      glVertex3f(-1.0,-1.0,0.0);
      glVertex3f(-1.0,+1.0,0.0);
      glVertex3f(+1.0,+1.0,0.0);
      glVertex3f(+1.0,-1.0,0.0);
   glEnd();

   glColor3f(1.0,1.0,1.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_TRIANGLES);
      glVertex3f(-0.8,-0.8,0.005);
      glVertex3f(-0.5,-0.1,0.005);
      glVertex3f(-0.2,-0.8,0.005);

      glVertex3f(-0.8,-0.8,-0.005);
      glVertex3f(-0.5,-0.1,-0.005);
      glVertex3f(-0.2,-0.8,-0.005);
   glEnd();
}

void figura_compleja()
{
   const float lado_simple = 2.0;
   const float diagonal_simple = sqrt(2*lado_simple*lado_simple);
   const float lado_nuevo = diagonal_simple;
   const float diagonal_nueva = sqrt(2*lado_nuevo*lado_nuevo);

   float ratio_incremento = lado_nuevo / lado_simple;

   cout << "Lado simple: " << lado_simple << endl
         << "Diagonal simple: " << diagonal_simple << endl
         << "Lado nuevo: " << lado_nuevo << endl
         << "Diagonal nueva: " << diagonal_nueva << endl;



   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

      glTranslatef(-1.5*lado_simple,-lado_simple/2,0.0);
      figura_simple();

   glPopMatrix();

   glPushMatrix();

      glRotatef(-45.0,0.0,0.0,1.0);
      glScalef(ratio_incremento,-ratio_incremento,0.0);

      figura_simple();

   glPopMatrix();

   glPushMatrix();

      glScalef(1.5*lado_simple,-1.0,0.0);
      //glTranslatef(+1.5*lado_simple,0.0,0.0);

      figura_simple();

   glPopMatrix();

}

void Problemas::DibujarObjetos()
{
   //gancho_x4();
   figura_compleja();
}

void Problemas::CambioModoDibujo(visualizacion modo_dibujo)
{
   malla->CambioModoDibujo(modo_dibujo);
}

void Problemas::CambioModoNormales()
{
   malla->CambioModoNormales();
}

void Problemas::CambioColorFijo()
{
   malla->CambioColorFijo();
}

bool Problemas::GestionarEvento(unsigned char tecla)
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

void Problemas::Debug()
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

void Problemas::Ayuda(vector<string> & strings_control)
{

}

void Problemas::FijarCamara()
{
   camara->FijarCamara();
}
void Problemas::FijarProyeccion(float ventana_tam_x, float ventana_tam_y)
{
   camara->FijarProyeccion(ventana_tam_x, ventana_tam_y);
}

void Problemas::ModificaEjeXCamara(float nuevo)
{
   camara->ModificaEjeX(nuevo);
}
void Problemas::ModificaEjeYCamara(float nuevo)
{
   camara->ModificaEjeY(nuevo);
}

void Problemas::ModificarEscala(int signo)
{
   camara->ModificarEscala(signo);
}
