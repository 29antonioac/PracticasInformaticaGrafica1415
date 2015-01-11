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
#include "PilaMatrices.hpp"
#include "IDs_Shaders.hpp"
#include "grafoescena.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

Practica1::Practica1()
{
   malla = nullptr;
}

Practica1::~Practica1()
{
   delete malla;
}

void Practica1::Inicializar( int argc, char *argv[] )
{
   if (argc == 1)
   {
      file = "./PLY/ant.ply";
   }
   else if (argc == 5)
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
   malla = new MallaTVT(MALLA,vertices_ply,caras_ply);

   cout << "-----------------------------------" << endl;
/*
   IDModelado = glGetUniformLocation(idProg_P1_P2,"Modelado");
   IDVista = glGetUniformLocation(idProg_P1_P2,"Vista");
   IDProyeccion = glGetUniformLocation(idProg_P1_P2,"Proyeccion");

   if (IDModelado == -1)
   {
      cout << "IDModelado es -1";
      glutLeaveMainLoop();
   }
   if (IDVista == -1)
   {
      cout << "IDVista es -1";
      glutLeaveMainLoop();
   }
   if (IDProyeccion == -1)
   {
      cout << "IDProyeccion es -1";
      glutLeaveMainLoop();
   }
   */
   raiz = new NodoGrafoEscena;
   NodoGrafoEscena * nodo_shader = new NodoShader(idProg_P1_P2);
   NodoGrafoEscena * nodo_malla = new NodoTerminal(malla);
   NodoGrafoEscena * nodo_transformacion = new NodoTransformacion(scale(glm::mat4(),glm::vec3(1.0,1.0,1.0)));

   raiz->aniadeHijo(nodo_shader);
      nodo_shader->aniadeHijo(nodo_transformacion);
         nodo_transformacion->aniadeHijo(nodo_malla);

   
}

// ---------------------------------------------------------------------
// función a implementar en la práctica 1  dibujar los objetos

void Practica1::DibujarObjetos()
{

   //UsarPrograma(idProg_P1_P2);
   /*
   glUniformMatrix4fv(IDModelado, 1, GL_FALSE, &Modelado[0][0]);
   glUniformMatrix4fv(IDVista, 1, GL_FALSE, &Vista[0][0]);
   glUniformMatrix4fv(IDProyeccion, 1, GL_FALSE, &Proyeccion[0][0]);*/

   //malla->Visualizar();
   //cout << "idProg_actual: " << idProg_actual << ", idProg_P1_P2: " << idProg_P1_P2 << endl;
   raiz->Procesa();
   //cout << "idProg_actual: " << idProg_actual << ", idProg_P1_P2: " << idProg_P1_P2 << endl;


}

void Practica1::CambioModoDibujo(visualizacion modo_dibujo)
{
   malla->CambioModoDibujo(modo_dibujo);
}

void Practica1::CambioModoNormales()
{
   malla->CambioModoNormales();
}

void Practica1::CambioColorFijo()
{
   malla->CambioColorFijo();
}

bool Practica1::GestionarEvento(unsigned char tecla)
{
   bool redisp = true;
   switch(toupper(tecla))
   {
   // Dejamos implementado el switch por si añadimos alguna tecla a esta práctica en particular
      default:
         redisp = false;
         break;
   }
   return redisp;
}

void Practica1::Debug()
{

   string str_color_fijo;
   if (malla->ColorFijo())
      str_color_fijo = "Si";
   else
      str_color_fijo = "No";

   string cauce;

   if (idProg_actual == 0)
      cauce = "Fijo";
   else
      cauce = "Programable";

   vector<string> debug_strings;
   debug_strings.push_back(string("Modo de normales: " + enumToString(malla->getModoNormales())));
   debug_strings.push_back(string("Color fijo: " + str_color_fijo));
   debug_strings.push_back(string("Modo de dibujo: " + enumToString(malla->getModoDibujo())));
   debug_strings.push_back(string("Archivo cargado: " + file));
   debug_strings.push_back(string("Cauce: " + cauce));
   debug_strings.push_back(string("Practica 1"));
   void * font = GLUT_BITMAP_9_BY_15;
   unsigned num_lineas = 0;
   for (auto &s: debug_strings)
   {
      glRasterPos2i(10, 10+15*num_lineas);
      for (auto &c: s)
      {
        glutBitmapCharacter(font, c);
      }
      num_lineas++;

   }

}

void Practica1::Ayuda(vector<string> & strings_control)
{

}
