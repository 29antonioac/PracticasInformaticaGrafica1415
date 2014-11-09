
#ifndef IG_MATRIZ_HPP
#define IG_MATRIZ_HPP

#include "tuplas.hpp"

class Matriz4x4
{
private:
   float coe[4][4];
public:
   Matriz4x4();
   Matriz4x4(float m[4][4], bool traspuesta = false);
   Matriz4x4(const Matriz4x4& m);

   Matriz4x4& operator=(const Matriz4x4& m);
   Matriz4x4 operator*(Matriz4x4& m);
   //Tupla4f operator*(Tupla4f& t);
   Tupla4f operator*(Tupla4f& t);
   Tupla3f operator*(Tupla3f& t);

   //Matriz4x4 trasponer();
};




#endif
