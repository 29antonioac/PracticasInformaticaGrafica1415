
#ifndef IG_MATRIZ_HPP
#define IG_MATRIZ_HPP

#include "tuplas.hpp"
#include <GL/gl.h>

class Matriz4x4
{
private:
   float coe[4][4];
public:
   Matriz4x4();
   Matriz4x4(float m[4][4], bool traspuesta = false);
   Matriz4x4(GLdouble m[16], bool traspuesta = false);
   Matriz4x4(const Matriz4x4& m);

   Matriz4x4& operator=(const Matriz4x4& m);
   Matriz4x4 operator*(const Matriz4x4& m);
   Tupla4f operator*(Tupla4f& t);
   Tupla3f operator*(Tupla3f& t);

   float * data();

   static Matriz4x4 Identidad();

   static Matriz4x4 RotacionEjeX(const float angulo);
   static Matriz4x4 RotacionEjeY(const float angulo);
   static Matriz4x4 RotacionEjeZ(const float angulo);

   static Matriz4x4 RotacionEje(const float angulo, const float ex, const float ey, const float ez);
   static Matriz4x4 RotacionEje(const float angulo, const Tupla3f eje);
   //static Matriz4x4 RotacionPunto(const float angulo, const Tupla3f punto, const Tupla3f origen);

   static Matriz4x4 Escalado(const float sx, const float sy, const float sz);
   static Matriz4x4 Escalado(const Tupla3f parametros);

   static Matriz4x4 Traslacion(const float dx, const float dy, const float dz);
   static Matriz4x4 Traslacion(const Tupla3f parametros);

};




#endif
