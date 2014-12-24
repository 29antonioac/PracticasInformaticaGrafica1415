// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 4  (implementación)
// **
// *********************************************************************

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "practica4.hpp"

#include "MallaTVT.hpp"
#include "file_ply_stl.hpp"
#include "Matriz.hpp"
#include "FuenteLuz.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::to_string;

template <class T>
inline T signo (T valor)
{
   if (valor > 0)
      return 1;
   else if (valor < 0)
      return -1;
   else
      return 0;
}

Practica4::Practica4()
{
   modo_dibujo = ALAMBRE;
   /*
   raiz = NULL;
   semiesfera = NULL;
   cilindro = NULL;

   angulo_rotacion_cuerpo = M_PI;
   angulo_rotacion_brazos = M_PI;
   angulo_rotacion_piernas = M_PI;

   direccion_rotacion_brazos = direccion_rotacion_piernas = 1;
   distancia_eje_Y = velocidad_angular_cuerpo = velocidad_angular_brazos = velocidad_angular_piernas = 0;

   rotacion_cuerpo = rotacion_brazo_izquierdo = rotacion_brazo_derecho = rotacion_pierna_izquierda = rotacion_pierna_derecha = traslacion = NULL;
*/

}

Practica4::~Practica4()
{
	/*
   delete raiz;
   delete semiesfera;
   delete cilindro;
   delete rotacion_cuerpo;
   delete rotacion_brazo_izquierdo;
   delete rotacion_brazo_derecho;
   delete rotacion_pierna_izquierda;
   delete rotacion_pierna_derecha;
   delete traslacion;
   */
}

void Practica4::Inicializar( int argc, char *argv[] )
{

   // Luces, cámara y acción!

   glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
   glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR ) ;
   glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );

   // Cargar ply del cuerpo de la lata





}


void Practica4::DibujarObjetos()
{
   switch (modo_dibujo)
   {
      case ALAMBRE:
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         break;
      case PUNTOS:
         glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
         break;
      case SOLIDO:
      case AJEDREZ:
      case SOLIDO_CARAS:
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         break;
      default:
         cout << "Enumerado inválido para modo_dibujo" << endl;
         exit(-3);
         break;

   }

   glEnable( GL_LIGHTING );
   glEnable( GL_NORMALIZE );
   glDisable( GL_COLOR_MATERIAL );

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

   glPopMatrix();

   glDisable( GL_LIGHTING );
   glDisable( GL_NORMALIZE );
   glEnable( GL_COLOR_MATERIAL );

}

void Practica4::CambioModoDibujo(visualizacion modo_dibujo)
{
   this->modo_dibujo = modo_dibujo;
}

void Practica4::CambioModoNormales()
{

}


void Practica4::CambioGradoLibertad(int grado_libertad)
{

   if (grado_libertad != 1 && grado_libertad != -1
         && grado_libertad != 2 && grado_libertad != -2
         && grado_libertad != 3 && grado_libertad != -3
         && grado_libertad != 4 && grado_libertad != -4)
   {
      cout << "Grado de libertad inválido" << endl;
      exit(-4);
   }



}

void Practica4::CambioColorFijo()
{

}

bool Practica4::GestionarEvento(unsigned char tecla)
{
   bool redisp = true;

   switch(tecla)
   {
      // Control de grados de libertad

      default:
         redisp = false;
         break;
   }
   return redisp;
}


void Practica4::Debug()
{
   string str_color_fijo;
   /*if (semiesfera->ColorFijo())
      str_color_fijo = "Si";
   else
      str_color_fijo = "No";
      */

   vector<string> debug_strings;
   /*
   debug_strings.push_back(string("Velocidad angular piernas: " + to_string(velocidad_angular_piernas)));
   debug_strings.push_back(string("Velocidad angular brazos: " + to_string(velocidad_angular_brazos)));
   debug_strings.push_back(string("Velocidad angular cuerpo: " + to_string(velocidad_angular_cuerpo)));
   debug_strings.push_back(string("Angulo de rotacion piernas: " + to_string(angulo_rotacion_piernas)));
   debug_strings.push_back(string("Angulo de rotacion brazos: " + to_string(angulo_rotacion_brazos)));
   debug_strings.push_back(string("Angulo de rotacion cuerpo: " + to_string(angulo_rotacion_cuerpo)));
   debug_strings.push_back(string("Distancia al eje Y: " + to_string(distancia_eje_Y)));
   debug_strings.push_back(string("Modo de normales: " + enumToString(semiesfera->getModoNormales())));
   debug_strings.push_back(string("Color fijo: " + str_color_fijo));
   debug_strings.push_back(string("Modo de dibujo: " + enumToString(modo_dibujo)));
   debug_strings.push_back(string("Practica 3"));
   */
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

void Practica4::Ayuda(vector<string> & strings_control)
{
   strings_control.push_back("M/m para modificar velocidad de rotacion de las piernas");
   strings_control.push_back("N/n para modificar velocidad de rotacion de los brazos");
   strings_control.push_back("B/b para modificar velocidad de rotacion del cuerpo");
   strings_control.push_back("V/v para modificar distancia al eje Y");
   strings_control.push_back("C/c para modificar rotacion de las piernas");
   strings_control.push_back("X/x para modificar rotacion de los brazos");
   strings_control.push_back("Z/z para modificar rotacion del cuerpo");


}
