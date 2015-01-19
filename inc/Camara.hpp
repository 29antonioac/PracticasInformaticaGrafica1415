#ifndef CAMARA_HPP
#define CAMARA_HPP

#include <GL/glew.h>
#include "enumerados.hpp"
#include "tuplas.hpp"

class Camara
{
private:
   tipo_camara tipo;
   modo_camara modo;

   Tupla3f posicion_observador, posicion_punto_atencion;
   float camara_rotacion_x, camara_rotacion_y;

   float factor_escala;

   static unsigned numero_camaras;
   static const float left, right, top, bottom, near, far;

public:
   Camara();
   Camara(tipo_camara tipo, modo_camara modo, Tupla3f posicion_observador, Tupla3f posicion_punto_atencion,
         float rotacion_eje_X, float rotacion_eje_Y);
   void FijarCamara();
   void FijarProyeccion(float ventana_tam_x, float ventana_tam_y);
   void ModificaEjeX(float nuevo);
   void ModificaEjeY(float nuevo);
   void ModificarEscala(int signo);


};



#endif
