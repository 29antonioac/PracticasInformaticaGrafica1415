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

unsigned N;

// ---------------------------------------------------------------------
// función para implementar en la práctica 1 para inicialización
// se llama una vez al inicio, cuando ya se ha creado la ventana e 
// incializado OpenGL. El PLY se debe cargar aquí.

void P2_Inicializar( int argc, char *argv[] )
{

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

   std::cout << "Tabla de vértices" << std::endl;

   std::vector<int> caras;

   ply::read_vertices(file.c_str(),vertices_ply);
   std::cout << "Tabla de vértices" << std::endl;

   for (unsigned i = 0; i < vertices_ply.size(); i+=3)
   {
      std::cout << "(" << vertices_ply[i] << "," << vertices_ply[i+1] << "," << vertices_ply[i+2] << ")" << std::endl;
   }

   pm_P2 = new MallaTVT(vertices_ply,ALAMBRE);
   pm_P2 = pm_P2->MTVT_Revolucion(N);

   std::cout << "-----------------------------------" << std::endl;


   
}

// ---------------------------------------------------------------------
// función a implementar en la práctica 1  dibujar los objetos

void P2_DibujarObjetos()
{
   pm_P2->MTVT_Visualizar();

   /*glBegin(GL_LINE_STRIP);
   for (unsigned i = 0; i < vertices_ply.size(); i+=3)
      glVertex3f(vertices_ply[i], vertices_ply[i+1], vertices_ply[i+2]);
   glEnd();*/

   /*unsigned num_ver = vertices_ply.size()/3;
   //std::cout << "Número de vértices " << vertices.size() << std::endl;

   for (unsigned j = 0; j < vertices.size(); j++)
   {
      //std::cout << "Entro en begin! " << std::endl;
      glBegin(GL_LINE_STRIP);
      //for (unsigned i = 0; i < vertices.size(); i+=3)
      //   glVertex3f( vertices[i],vertices[i+1],vertices[i+2] );
      //for (unsigned i = 0; i < perfiles.at(0).size(); i++)
      for (unsigned i = 0; i < num_ver; i++)
      {
         //std::cout << perfiles.at(j).at(i) << " ";
        glVertex3f( vertices[j*num_ver+i][0], vertices[j*num_ver+i][1], vertices[j*num_ver+i][2]);
      }
      //std::cout << std::endl;

      glEnd();

   }*/


}
