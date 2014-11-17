// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 1  (implementación)
// **
// *********************************************************************

// Práctica 2 NO IMPLEMENTADA
// Está aquí sólo por motivos de prueba
// del sistema de cambio de prácticas

//#define c cos(alpha)
//#define s sin(alpha)

#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "practica2.hpp"
#include "file_ply_stl.hpp"
#include "mallaTVT.hpp"
#include "Matriz.hpp"

std::vector<float> P2_vertices_ply;

MallaTVT * pm_P2;

// ---------------------------------------------------------------------
// Estos defines controlan si la práctica se compila para modo revolución,
// barrido por rotación o barrido por traslación.
// Si hay más de un define tiene prioridad el que esté más arriba.
//


#define REVOLUCION
//#define BARRIDO_ROTACION
//#define BARRIDO_TRASLACION

void P2_Inicializar( int argc, char *argv[] )
{

   unsigned N;
   std::string file;

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
      file = std::string(argv[2]);
      N = atoi(argv[3]);

      if (N < 3)
      {
         std::cout << "Por favor, introduce N >= 3" << std::endl;
         exit(-1);
      }
      else if (N >= 10000)
      {
         std::cout << "N muy grande, puede tardar un poco en procesar..." << std::endl;
      }
   }
   else
   {
      std::cout << "Argumentos mal (argc = " << argc << ")" << std::endl;
      exit(-1);
   }

   std::cout << "Archivo: " << file << std::endl;

   ply::read_vertices(file.c_str(),P2_vertices_ply);

#if defined REVOLUCION or defined BARRIDO_ROTACION or defined BARRIDO_TRASLACION
   pm_P2 = new MallaTVT(P2_vertices_ply);
#endif

#if defined REVOLUCION
   pm_P2 = pm_P2->Revolucion(N);
#elif defined BARRIDO_ROTACION
   pm_P2 = pm_P2->Barrido_Rotacion(N);
#elif defined BARRIDO_TRASLACION
   pm_P2 = pm_P2->Barrido_Traslacion(N,0.5,0.5,0.5);
#endif

#if not defined REVOLUCION and not defined BARRIDO_ROTACION and not defined BARRIDO_TRASLACION
for (unsigned i = 0; i < P2_vertices_ply.size(); i+=3)
{
   std::cout << "(" << P2_vertices_ply[i] << "," << P2_vertices_ply[i+1] << "," << P2_vertices_ply[i+2] << ")" << std::endl;
}
#endif

   std::cout << "-----------------------------------" << std::endl;


   
}

// ---------------------------------------------------------------------
// función a implementar en la práctica 1  dibujar los objetos

void P2_DibujarObjetos()
{


#if defined REVOLUCION or defined BARRIDO_ROTACION or defined BARRIDO_TRASLACION
   pm_P2->Visualizar();
#endif

#if not defined REVOLUCION and not defined BARRIDO_ROTACION and not defined BARRIDO_TRASLACION
   glBegin(GL_LINE_STRIP);
   for (unsigned i = 0; i < P2_vertices_ply.size(); i+=3)
   {
      glVertex3f (P2_vertices_ply[i], P2_vertices_ply[i+1], P2_vertices_ply[i+2] );
   }
   glEnd();
#endif





}
