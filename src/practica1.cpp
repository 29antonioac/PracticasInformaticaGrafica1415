// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 1  (implementación)
// **
// *********************************************************************

#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "practica1.hpp"
#include "file_ply_stl.hpp"
#include "mallaTVT.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

// ---------------------------------------------------------------------
// coordenadas de los vértices del cubo:

const unsigned num_verts_cubo = 8 ;

GLfloat coords_verts_cubo[num_verts_cubo][3] = 
   {  { -0.5, -0.5, +0.5 } ,
      { +0.5, -0.5, +0.5 } ,
      { +0.5, +0.5, +0.5 } ,
      { -0.5, +0.5, +0.5 } ,
      { -0.5, -0.5, -0.5 } ,
      { +0.5, -0.5, -0.5 } ,
      { +0.5, +0.5, -0.5 } ,
      { -0.5, +0.5, -0.5 }
   };


MallaTVT * pm_P1;
   
// ---------------------------------------------------------------------
//  dibujar vértices del cubo en modo inmediato con begin/end

void DibujarCuboPuntos()
{
   glColor3f( 0.10, 0.15, 0.40 );
   glPointSize(4);

   glBegin(GL_POINTS);
   for ( unsigned i= 0 ; i < num_verts_cubo ; i++ )
   {
        glVertex3fv( &( coords_verts_cubo[i][0] ) );
   }
   glEnd();
   
}

// ---------------------------------------------------------------------
// función para implementar en la práctica 1 para inicialización
// se llama una vez al inicio, cuando ya se ha creado la ventana e 
// incializado OpenGL. El PLY se debe cargar aquí.

void P1_Inicializar( int argc, char *argv[] )
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

   vector<float> vertices;
   vector<int> caras;

   ply::read(file.c_str(),vertices,caras);
   pm_P1 = new MallaTVT(vertices,caras);

   cout << "-----------------------------------" << endl;


   
}

// ---------------------------------------------------------------------
// función a implementar en la práctica 1  dibujar los objetos

void P1_DibujarObjetos()
{
   //DibujarCuboPuntos() ;
   pm_P1->Visualizar();
}
