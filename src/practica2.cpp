// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include <cstdlib>
#include <iostream>
#include <string>
#include "practica2.hpp"
#include "file_ply_stl.hpp"
#include "Matriz.hpp"

using std::cout;
using std::vector;
using std::string;

Practica2::Practica2()
{
   malla = NULL;
}

Practica2::~Practica2()
{
   delete malla;
}

void Practica2::Inicializar(int argc, char *argv[])
{

   unsigned N,M;
   string file;

   if (argc == 1)
   {
      file = "PLY/arboldenavidad.ply";
      N = 20;
      M = 0;
   }
   else if (argc == 5)
   {
      file = string(argv[2]);
      N = atoi(argv[3]);
      M = atoi(argv[4]);

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


   malla = new MallaTVT(vertices_ply);


   if (M == 0)
      malla = malla->Revolucion(N);
   else if (M == 1)
      malla = malla->Barrido_Rotacion(N);
   else if (M == 2)
      malla = malla->Barrido_Traslacion(N,0.5,0.5,0.5);

   cout << "-----------------------------------" << endl;
}

void Practica2::DibujarObjetos()
{
   malla->Visualizar();
}

void Practica2::CambioModoDibujo(visualizacion modo_dibujo)
{
   malla->CambioModoDibujo(modo_dibujo);
}

void Practica2::CambioModoNormales()
{
   malla->CambioModoNormales();
}

void Practica2::CambioColorFijo()
{
   malla->CambioColorFijo();
}

bool Practica2::GestionarEvento(unsigned char tecla)
{
   bool redisp = true;
   switch(toupper(tecla))
   {
      case 'L':
         CambioModoDibujo(ALAMBRE);
         break;
      case 'S':
         CambioModoDibujo(SOLIDO);
         break;
      case 'D':
         CambioModoDibujo(SOLIDO_CARAS);
         break;
      case 'P':
         CambioModoDibujo(PUNTOS);
         break;
      case 'A':
         CambioModoDibujo(AJEDREZ);
         break;
      case 'N':
         CambioModoNormales();
         break;
      case 'F':
         CambioColorFijo();
         break;
      default:
         redisp = false;
         break;
   }
   return redisp;
}
