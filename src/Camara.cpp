#include "Camara.hpp"
#include "error-ogl.hpp"
#include <cassert>

unsigned Camara::numero_camaras = 0;
const float Camara::left = -0.1,
            Camara::right = 0.1,
            Camara::bottom = -0.1,
            Camara::top = 0.1,
            Camara::near = 0.1,
            Camara::far = 10.0;

/*
 * glFrustum
   (  -frustum_ancho,
      +frustum_ancho,
      -frustum_ancho*ratioYX,
      +frustum_ancho*ratioYX,
      +frustum_dis_del,
      +frustum_dis_tra
   );
      // inicializar parámetros del frustum
   frustum_dis_del         = 0.1 ;
   frustum_dis_tra         = 10.0;
   frustum_ancho           = 0.5*frustum_dis_del ;
   //frustum_ancho           = 1.0 ;
   frustum_factor_escala   = 1.0 ;
 */

Camara::Camara()
{
   this->tipo = PERSPECTIVA;
   this->modo = PRIMERA_PERSONA;

   this->posicion_observador = Tupla3f(0.0,0.0,-0.8*(Camara::near + Camara::far));
   this->posicion_punto_atencion = Tupla3f();
   this->camara_rotacion_x = this->camara_rotacion_y = 0;

   this->factor_escala = 1.0;

   numero_camaras++;
}

Camara::Camara(tipo_camara tipo, modo_camara modo, Tupla3f posicion_observador, Tupla3f posicion_punto_atencion,
         float camara_rotacion_x, float camara_rotacion_y)
{
   this->tipo = tipo;
   this->modo = modo;

   this->posicion_observador = posicion_observador;
   this->posicion_punto_atencion = posicion_punto_atencion;
   this->camara_rotacion_x = camara_rotacion_x;
   this->camara_rotacion_y = camara_rotacion_y;

   this->factor_escala = 1.0;

   numero_camaras++;
}

void Camara::FijarCamara()
{
   CError();
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glRotatef(camara_rotacion_x,1,0,0);
   glRotatef(camara_rotacion_y,0,1,0);

   CError();
}
void Camara::FijarProyeccion(float ventana_tam_x, float ventana_tam_y)
{
   const GLfloat ratioYX = GLfloat( ventana_tam_y )/GLfloat( ventana_tam_x );

   CError();

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   if (tipo == PERSPECTIVA)
   {
      glFrustum
      (  left,
         right,
         bottom*ratioYX,
         top*ratioYX,
         near,
         far
      );
   }
   else
   {
      glOrtho
      (  left,
         right,
         bottom*ratioYX,
         top*ratioYX,
         near,
         far
      );
   }

   if (modo != EXAMINAR)
      glTranslatef( posicion_observador[X], posicion_observador[Y], posicion_observador[Z]);
   else
      glTranslatef( posicion_punto_atencion[X], posicion_punto_atencion[Y], posicion_punto_atencion[Z]);

   glScalef(factor_escala, factor_escala, factor_escala);

   CError();
}

void Camara::ModificaEjeX(float nuevo)
{
   this->camara_rotacion_x += nuevo;
}
void Camara::ModificaEjeY(float nuevo)
{
   this->camara_rotacion_y += nuevo;
}

void Camara::ModificarEscala(int signo)
{
   assert (signo == 1 || signo == -1);

   if (signo == 1)   this->factor_escala *= 1.05;
   else              this->factor_escala /= 1.05;


}
