#include "Matriz.hpp"
//#include "tuplas2.hpp"

Matriz4x4::Matriz4x4()
{
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         coe[i][j] = 0;
}

Matriz4x4::Matriz4x4(float m[4][4])
{
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         coe[i][j] = m[i][j];
}

Matriz4x4::Matriz4x4(const Matriz4x4& m)
{
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         coe[i][j] = m.coe[i][j];
}

Matriz4x4& Matriz4x4::operator=(const Matriz4x4& m)
{
   if (this != &m)
   {
      for (int i = 0; i < 4; i++)
         for (int j = 0; j < 4; j++)
            coe[i][j] = m.coe[i][j];
   }
   return *this;
}

Matriz4x4 Matriz4x4::operator*(Matriz4x4& m)
{
   Matriz4x4 resultado;

   for (unsigned fila = 0; fila < 4; fila++)
   {
      for (unsigned col = 0; col < 4; col++)
      {
         resultado.coe[col][fila] = 0.0f;
         for (unsigned k = 0; k < 4; k++)
         {
            resultado.coe[col][fila] += this->coe[k][fila]*m.coe[col][k];
         }
      }
   }

   return resultado;


}

Tupla4f Matriz4x4::operator*(Tupla4f& t)
{
   Tupla4f resultado;

   for (unsigned fila = 0; fila < 4; fila++)
   {
      resultado[fila] = 0.0f;
      for (unsigned col = 0; col < 4; col++)
      {
         resultado[fila] += this->coe[col][fila]*t[col];
      }
   }

   return resultado;
}

Tupla3f Matriz4x4::operator*(Tupla3f& t)
{
   Tupla4f resultado;
   Tupla4f t2 = Tupla4f(t[0],t[1],t[2],1);

   for (unsigned fila = 0; fila < 4; fila++)
   {
      resultado[fila] = 0.0f;
      for (unsigned col = 0; col < 4; col++)
      {
         resultado[fila] += this->coe[fila][col]*t2[col];
      }
   }

   return Tupla3f(resultado[0],resultado[1],resultado[2]);
}


//Matriz4x4 Matriz4x4::trasponer();
