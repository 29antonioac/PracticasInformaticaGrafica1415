#include "Camara.hpp"
#include "error-ogl.hpp"
#include "Matriz.hpp"
#include <cassert>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

unsigned Camara::numero_camaras = 0;
const float Camara::left = -0.1,
            Camara::right = 0.1,
            Camara::bottom = -0.1,
            Camara::top = 0.1,
            Camara::near = 0.1,
            Camara::far = 100.0;

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

//Compat method: gluLookAt deprecated
void util_compat_gluLookAt(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat lookAtX, GLfloat lookAtY, GLfloat lookAtZ, GLfloat upX, GLfloat upY, GLfloat upZ) {
    Tupla3f x, y, z;
    z = Tupla3f(eyeX-lookAtX, eyeY-lookAtY, eyeZ-lookAtZ).normalized();
    y = Tupla3f(upX, upY, upZ);
    x = y * z;
    y = z * x;
    x = x.normalized();
    y = y.normalized();
    // mat is given transposed so OpenGL can handle it.
    GLfloat mat[16] =
                     {x[X], y[X],   z[X],   0,
                     x[Y],  y[Y],   z[Y],   0,
                     x[Z],  y[Z],   z[Z],   0,
                     -eyeX,     -eyeY,      -eyeZ,      1};

    glMultMatrixf(mat);
}


Camara::Camara()
{
   this->tipo = PERSPECTIVA;
   this->modo = EXAMINAR;

   this->posicion_observador = Tupla3f(0.0,0.0,10.0);
   this->posicion_observador_inicial = Tupla3f(0.0,0.0,10.0);

   this->posicion_punto_atencion = Tupla3f();
   this->posicion_punto_atencion_inicial = Tupla3f();

   this->camara_rotacion_x = 0;
   this->camara_rotacion_y = 0;

   this->factor_zoom = 0.0;

   numero_camaras++;
}

Camara::Camara(tipo_camara tipo)
{
   this->tipo = tipo;
   this->modo = PRIMERA_PERSONA;

   this->posicion_observador = Tupla3f(0.0,0.0,10.0);
   this->posicion_observador_inicial = Tupla3f(0.0,0.0,10.0);

   this->posicion_punto_atencion = Tupla3f();
   this->posicion_punto_atencion_inicial = Tupla3f();

   this->camara_rotacion_x = 0;
   this->camara_rotacion_y = 0;

   this->factor_zoom = 0.0;

   numero_camaras++;
}

Camara::Camara(modo_camara modo)
{
   this->tipo = PERSPECTIVA;
   this->modo = modo;

   this->posicion_observador = Tupla3f(0.0,0.0,10.0);
   this->posicion_observador_inicial = Tupla3f(0.0,0.0,10.0);

   this->posicion_punto_atencion = Tupla3f();
   this->posicion_punto_atencion_inicial = Tupla3f();

   this->camara_rotacion_x = 0;
   this->camara_rotacion_y = 0;

   this->factor_zoom = 0.0;

   numero_camaras++;
}

Camara::Camara(tipo_camara tipo, modo_camara modo, Tupla3f posicion_observador, Tupla3f posicion_punto_atencion,
         float camara_rotacion_x, float camara_rotacion_y)
{
   this->tipo = tipo;
   this->modo = modo;

   this->posicion_observador = posicion_observador;
   this->posicion_observador_inicial = posicion_observador;
   this->posicion_punto_atencion = posicion_punto_atencion;
   this->posicion_punto_atencion_inicial = posicion_punto_atencion;
   this->camara_rotacion_x = camara_rotacion_x;
   this->camara_rotacion_y = camara_rotacion_y;

   this->factor_zoom = 1.0;

   numero_camaras++;
}



void Camara::FijarCamara()
{
   CError();
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();


   if (modo == EXAMINAR)
   {
      Tupla3f zoom (0.0,0.0,factor_zoom);
      //Tupla3f zoom;
      Tupla3f tmp = posicion_observador_inicial + zoom;
      posicion_observador = Matriz4x4::RotacionEjeY(-camara_rotacion_y) * Matriz4x4::RotacionEjeX(-camara_rotacion_x) * tmp;
      //posicion_observador = Matriz4x4::RotacionEjeX(-camara_rotacion_x) * Matriz4x4::RotacionEjeY(-camara_rotacion_y) * tmp;
   }

   else  // Modo Primera persona
   {
      Tupla3f vector_ojo_foco = posicion_punto_atencion_inicial - posicion_observador;
      vector_ojo_foco = Matriz4x4::RotacionEjeY(-camara_rotacion_y) * Matriz4x4::RotacionEjeX(-camara_rotacion_x) * vector_ojo_foco;
      posicion_punto_atencion = posicion_observador + vector_ojo_foco;
      //posicion_punto_atencion_inicial = posicion_punto_atencion;
   }



   gluLookAt(posicion_observador[X], posicion_observador[Y], posicion_observador[Z],
               posicion_punto_atencion[X], posicion_punto_atencion[Y], posicion_punto_atencion[Z],
               0.0,1.0,0.0);


   /*
   glm::vec3 eye(posicion_observador[X], posicion_observador[Y], posicion_observador[Z]);
   glm::vec3 PoV(posicion_punto_atencion[X], posicion_punto_atencion[Y], posicion_punto_atencion[Z]);
   glm::vec3 up(0.0,1.0,0.0);

   glm::mat4 matriz_lookAt = glm::lookAt(eye,PoV,up);

   glMultMatrixf(&matriz_lookAt[0][0]);
   */
/*
   glRotatef(-camara_rotacion_y, 0.0,1.0,0.0);
   glRotatef(-camara_rotacion_x, 1.0,0.0,0.0);
   glTranslatef(posicion_observador[X], posicion_observador[Y], posicion_observador[Z]);
   */




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


   CError();
}

Tupla3f Camara::getObservador()
{
   return posicion_observador;
}

Tupla3f Camara::getPuntoAtencion()
{
   return posicion_punto_atencion;
}

float Camara::getEjeX()
{
   return camara_rotacion_x;
}

float Camara::getEjeY()
{
   return camara_rotacion_y;
}

void Camara::ModificaEjeX(float incremento)
{
   this->camara_rotacion_x += incremento;

   // Limitamos a [-Pi,Pi] la cámara en Y para que no dé saltos.
   // Le quitamos 0.001 para que no llegue al límite
   if (this->camara_rotacion_x <= -M_PI/2) this->camara_rotacion_x = -M_PI/2 + 0.001;
   else if (this->camara_rotacion_x >= M_PI/2) this->camara_rotacion_x = M_PI/2 - 0.001;
}
void Camara::ModificaEjeY(float incremento)
{
   this->camara_rotacion_y += incremento;

}

void Camara::ModificarEscala(int signo)
{
   assert (signo == 1 || signo == -1);

   if (signo == 1)   this->factor_zoom -= 1.0;
   else              this->factor_zoom += 1.0;

   if (this->factor_zoom <= 0) this->factor_zoom = 0;


}

void Camara::ModificarPosicionX(float incremento)
{
   if (modo == PRIMERA_PERSONA)
   {
      Tupla3f PoV_Obs = posicion_punto_atencion - posicion_observador;
      Tupla3f direccion = Tupla3f(PoV_Obs[Z],0.0,-PoV_Obs[X]).normalized();   // Mira hacia la derecha
      //std::cout << "Vector direccionX: " << direccion << std::endl;
      posicion_observador += direccion * incremento;
      posicion_punto_atencion_inicial += direccion * incremento;
   }
}

void Camara::ModificarPosicionZ(float incremento)
{
   if (modo == PRIMERA_PERSONA)
   {
      Tupla3f PoV_Obs = posicion_punto_atencion - posicion_observador;
      //std::cout << "Vector direccionZ: " << PoV_Obs << std::endl;
      Tupla3f direccion = Tupla3f(PoV_Obs[X],0.0,PoV_Obs[Z]).normalized();   // Mira hacia la derecha
      posicion_observador += direccion * incremento;
      posicion_punto_atencion_inicial += direccion * incremento;
   }
}

void Camara::SetPuntoAtencion(Tupla3f punto_atencion)
{
   this->posicion_punto_atencion = punto_atencion;
}

void Camara::SetModo(modo_camara modo)
{
   this->modo = modo;
}

void Camara::ReiniciarPosicion()
{
   std::cout << "Reiniciar" << std::endl;
   this->posicion_observador = this->posicion_observador_inicial;
   this->posicion_punto_atencion = this->posicion_punto_atencion_inicial;
}
