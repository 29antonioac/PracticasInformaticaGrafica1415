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

// ---------------------------------------------------------------------
// función para implementar en la práctica 1 para inicialización
// se llama una vez al inicio, cuando ya se ha creado la ventana e 
// incializado OpenGL. El PLY se debe cargar aquí.

void Practica1::Inicializar( int argc, char *argv[] )
{

   string file;
   
   if (argc == 1)
   {
      file = "./PLY/ant.ply";
   }
   else if (argc == 4)
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
   //DibujarCuboPuntos() ;
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
