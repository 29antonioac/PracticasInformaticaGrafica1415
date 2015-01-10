// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// **
// ** Práctica 3  (declaraciones públicas)
// **
// *********************************************************************

#ifndef __IG_PRACTICA3_HPP__
#define __IG_PRACTICA3_HPP__

#include "practica.hpp"
//#include "tuplas.hpp"
#include "grafoescena.hpp"
#include "FuenteLuz.hpp"
#include "Material.hpp"
#include <glm/mat4x4.hpp>

class Practica3 : virtual public Practica
{
public:

   void DibujarObjetos();
   void Inicializar(int argc, char * argv[]);
   void CambioModoDibujo(visualizacion modo_dibujo);
   void CambioModoNormales();
   void CambioGradoLibertad(int grado_libertad);
   void Animar();
   void CambioColorFijo();
   bool GestionarEvento(unsigned char tecla);
   void Debug();
   void Ayuda(vector<string> & strings_control);

   virtual ~Practica3();

   Practica3();
   Practica3(Practica3 const&);              // No se implementa para evitar copias
   Practica3& operator=(Practica3 const&);   // No se implementa para evitar asignaciones

private:
   visualizacion modo_dibujo;
   NodoGrafoEscena * raiz;
   MallaTVT * semiesfera, * cilindro, * semiesfera_ojo;

   glm::mat4 * rotacion_brazo_izquierdo, * rotacion_brazo_derecho, * rotacion_pierna_izquierda, * rotacion_pierna_derecha,
      * rotacion_cuerpo, * traslacion;

   static constexpr float incremento_angulo_rotacion_cuerpo = 2*M_PI/100,
         incremento_angulo_rotacion_brazos = 2*M_PI/100,
         incremento_angulo_rotacion_piernas = 2*M_PI/100,
         incremento_velocidad_rotacion_cuerpo = 2*M_PI/500,
         incremento_velocidad_rotacion_brazos = 2*M_PI/500,
         incremento_velocidad_rotacion_piernas = 2*M_PI/500,
         incremento_traslacion_cuerpo = 0.25;

   float direccion_rotacion_brazos, direccion_rotacion_piernas;
   float distancia_eje_Y, angulo_rotacion_cuerpo, angulo_rotacion_brazos, angulo_rotacion_piernas;
   float velocidad_angular_cuerpo, velocidad_angular_brazos, velocidad_angular_piernas;

   // Luces y material (extensión de la práctica 4)

   FuenteLuzPosicional * fuente1, * fuente2, * fuente3, * fuente4;
   ColeccionFuentesLuz fuentes;
   Material * material_android, * material_ojo;

   vector<glm::vec3> vertices_semiesfera;

   GLint IDLuz;




};

#endif
