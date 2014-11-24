// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 3  (implementación)
// **
// *********************************************************************

#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "practica3.hpp"
#include "file_ply_stl.hpp"
#include "mallaTVT.hpp"
#include "Matriz.hpp"

using std::cout;
using std::vector;
using std::string;



MallaTVT * pm_P3;

void P3_Inicializar( int argc, char *argv[] )
{

   /*unsigned N;
   string file;

   if (argc == 1)
   {
      file = "PLY/arboldenavidad.ply";
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
   vector<float> P3_vertices_ply;

   ply::read_vertices(file.c_str(),P3_vertices_ply);


   pm_P3 = new MallaTVT(P3_vertices_ply);

   cout << "-----------------------------------" << endl;*/
}

void P3_DibujarObjetos()
{
   //pm_P3->Visualizar();

   glutSolidTeapot(10);







}
