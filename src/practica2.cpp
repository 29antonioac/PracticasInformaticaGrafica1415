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

std::vector<float> vertices_ply;
std::vector<std::vector<Tupla3f> > perfiles;
std::vector<Tupla3i> caras;

MallaTVT * pm_P2;

// ---------------------------------------------------------------------
// función para implementar en la práctica 1 para inicialización
// se llama una vez al inicio, cuando ya se ha creado la ventana e 
// incializado OpenGL. El PLY se debe cargar aquí.


#define REVOLUCION
//#define BARRIDO_ROTACION
//#define BARRIDO_TRASLACION
void P2_Inicializar( int argc, char *argv[] )
{

   unsigned N;
   std::string file;
   
   if (argc < 2)
   {
#if defined REVOLUCION
      file = "PLY/peon.ply";
      N = 20;
#elif defined BARRIDO_ROTACION
      file = "PLY/toro.ply";
      N = 20;
#elif defined BARRIDO_TRASLACION
      file = "PLY/prisma.ply";
      N = 5;
#endif
   }
   else if (argc < 3)
   {
      file = std::string(argv[1]);
      N = 10;
   }
   else
   {
      file = std::string(argv[1]);
      N = atoi(argv[2]);
   }

   std::cout << "Archivo: " << file << std::endl;

   ply::read_vertices(file.c_str(),vertices_ply);
#if defined REVOLUCION or defined BARRIDO_ROTACION or defined BARRIDO_TRASLACION
   pm_P2 = new MallaTVT(vertices_ply,ALAMBRE);
#endif

#if defined REVOLUCION
   pm_P2 = pm_P2->Revolucion(N);
#elif defined BARRIDO_ROTACION
   pm_P2 = pm_P2->Barrido_Rotacion(N);
#elif defined BARRIDO_TRASLACION
   pm_P2 = pm_P2->Barrido_Traslacion(N,0.5);
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


   /*glBegin(GL_POLYGON);
   for (unsigned i = 0; i < vertices_ply.size(); i+=3)
   {
      glVertex3f (vertices_ply[i], vertices_ply[i+1], vertices_ply[i+2] );
   }
   glEnd();*/

#if not defined REVOLUCION and not defined BARRIDO_ROTACION and not defined BARRIDO_TRASLACION
   glBegin(GL_LINE_STRIP);
   for (unsigned i = 0; i < vertices_ply.size(); i+=3)
   {
      glVertex3f (vertices_ply[i], vertices_ply[i+1], vertices_ply[i+2] );
   }
   glEnd();
#endif





}
