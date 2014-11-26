// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include "practica2.hpp"
#include "file_ply_stl.hpp"
#include "Matriz.hpp"

using std::cout;
using std::vector;
using std::string;

// ---------------------------------------------------------------------
// Estos defines controlan si la práctica se compila para modo revolución,
// barrido por rotación o barrido por traslación.
// Si hay más de un define tiene prioridad el que esté más arriba.
//

#define REVOLUCION
//#define BARRIDO_ROTACION
//#define BARRIDO_TRASLACION

void Practica2::Inicializar(int argc, char *argv[])
{

   unsigned N;
   string file;

   if (argc == 1)
   {

#if defined REVOLUCION
      file = "PLY/arboldenavidad.ply";
      N = 20;
#elif defined BARRIDO_ROTACION
      file = "PLY/toro.ply";
      N = 20;
#elif defined BARRIDO_TRASLACION
      file = "PLY/prisma.ply";
      N = 5;
#else
      file = "PLY/arboldenavidad.ply";
#endif
   }
   else if (argc == 4)
   {
      file = string(argv[2]);
      N = atoi(argv[3]);

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

#if defined REVOLUCION or defined BARRIDO_ROTACION or defined BARRIDO_TRASLACION
   malla = new MallaTVT(vertices_ply);
#endif

#if defined REVOLUCION
   malla = malla->Revolucion(N);
#elif defined BARRIDO_ROTACION
   malla = malla->Barrido_Rotacion(N);
#elif defined BARRIDO_TRASLACION
   malla = malla->Barrido_Traslacion(N,0.5,0.5,0.5);
#endif

#if not defined REVOLUCION and not defined BARRIDO_ROTACION and not defined BARRIDO_TRASLACION
for (unsigned i = 0; i < vertices_ply.size(); i+=3)
{
   cout << "(" << vertices_ply[i] << "," << vertices_ply[i+1] << "," << vertices_ply[i+2] << ")" << endl;
}
#endif

   cout << "-----------------------------------" << endl;
}

void Practica2::DibujarObjetos()
{
#if defined REVOLUCION or defined BARRIDO_ROTACION or defined BARRIDO_TRASLACION
   malla->Visualizar();
#endif

#if not defined REVOLUCION and not defined BARRIDO_ROTACION and not defined BARRIDO_TRASLACION
   glBegin(GL_LINE_STRIP);
   for (unsigned i = 0; i < vertices_ply.size(); i+=3)
   {
      glVertex3f (vertices_ply[i], vertices_ply[i+1], vertices_ply[i+2] );
   }
   glEnd();
#endif
}

void Practica2::CambioModoDibujo(visualizacion modo_dibujo)
{
   malla->CambioModoDibujo(modo_dibujo);
}

void Practica2::CambioModoNormales()
{
   malla->CambioModoNormales();
}
