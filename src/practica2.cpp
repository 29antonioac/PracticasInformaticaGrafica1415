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

#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "practica2.hpp"
#include "file_ply_stl.hpp"
#include "mallaTVT.hpp"

// ---------------------------------------------------------------------
// función para implementar en la práctica 1 para inicialización
// se llama una vez al inicio, cuando ya se ha creado la ventana e 
// incializado OpenGL. El PLY se debe cargar aquí.

void P2_Inicializar( int argc, char *argv[] )
{

   std::string file;
   
   if (argc < 2)
   {
      file = "./PLY/big_atc.ply";
   }
   else
   {
      file = std::string(argv[1]);
   }

   std::cout << "Archivo: " << file << std::endl;

   std::vector<float> vertices;
   std::vector<int> caras;

   ply::read(file.c_str(),vertices,caras);
   pm_P2 = new MallaTVT(vertices,caras,ALAMBRE);

   std::cout << "-----------------------------------" << std::endl;


   
}

// ---------------------------------------------------------------------
// función a implementar en la práctica 1  dibujar los objetos

void P2_DibujarObjetos()
{
   //DibujarCuboPuntos() ;
   pm_P2->MTVT_Visualizar();
}
