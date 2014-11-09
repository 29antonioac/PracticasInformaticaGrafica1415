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

unsigned caras_longitudinales = 10;

MallaTVT * pm_P2;

// ---------------------------------------------------------------------
// función para implementar en la práctica 1 para inicialización
// se llama una vez al inicio, cuando ya se ha creado la ventana e 
// incializado OpenGL. El PLY se debe cargar aquí.

void P2_Inicializar( int argc, char *argv[] )
{

   unsigned N;
   std::string file;
   
   if (argc < 2)
   {
      file = "./PLY/peon.ply";
      N = 10;
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

   pm_P2 = new MallaTVT(vertices_ply,ALAMBRE);
   pm_P2 = pm_P2->Revolucion(N);
   //pm_P2 = pm_P2->Barrido(N);

   std::cout << "-----------------------------------" << std::endl;


   
}

// ---------------------------------------------------------------------
// función a implementar en la práctica 1  dibujar los objetos

void P2_DibujarObjetos()
{
   pm_P2->Visualizar();
   //pm_P2->VisualizarNormalesCaras();
   //pm_P2->VisualizarNormalesVertices();

   /*
   glBegin(GL_POLYGON);
   for (unsigned i = 0; i < vertices_ply.size(); i+=3)
   {
      glVertex3f (vertices_ply[i], vertices_ply[i+1], vertices_ply[i+2] );
   }

   glEnd();
   */


}
