#include "Matriz.hpp"

Matriz4x4::Matriz4x4()
{
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         coe[i][j] = 0;
}

Matriz4x4::Matriz4x4(float m[4][4], bool traspuesta)
{
   if(traspuesta)
   {
         for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
               coe[j][i] = m[j][i];
   }
   else
   {
      for (int i = 0; i < 4; i++)
         for (int j = 0; j < 4; j++)
            coe[j][i] = m[i][j];
   }
}

Matriz4x4::Matriz4x4(GLdouble m[16], bool traspuesta)
{
   if(traspuesta)
   {
         for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
               coe[j][i] = m[4*j+i];
   }
   else
   {
      for (int i = 0; i < 4; i++)
         for (int j = 0; j < 4; j++)
            coe[j][i] = m[4*i+j];
   }
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

Matriz4x4 Matriz4x4::operator*(const Matriz4x4& m)
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
         resultado[fila] += this->coe[col][fila]*t2[col];
      }
   }

   return Tupla3f(resultado[0],resultado[1],resultado[2]);
}

float * Matriz4x4::data()
{
   return coe[0];
}

Matriz4x4 Matriz4x4::Identidad()
{
   float m[4][4] = {
         {1,0,0,0},
         {0,1,0,0},
         {0,0,1,0},
         {0,0,0,1}
   };

   return Matriz4x4(m,true);
}

Matriz4x4 Matriz4x4::RotacionEjeX(const float angulo)
{
   float c = cosf(angulo);
   float s = sinf(angulo);

   float m[4][4] = {
         {1,0,0,0},
         {0,c,-s,0},
         {0,s,c,0},
         {0,0,0,1}
   };

   return Matriz4x4(m,false);
}

Matriz4x4 Matriz4x4::RotacionEjeY(const float angulo)
{
   float c = cosf(angulo);
   float s = sinf(angulo);

   float m[4][4] = {
         {c,0,s,0},
         {0,1,0,0},
         {-s,0,c,0},
         {0,0,0,1}
   };

   return Matriz4x4(m,false);
}

Matriz4x4 Matriz4x4::RotacionEjeZ(const float angulo)
{
   float c = cosf(angulo);
   float s = sinf(angulo);

   float m[4][4] = {
         {c,-s,0,0},
         {s,c,0,0},
         {0,0,1,0},
         {0,0,0,1}
   };

   return Matriz4x4(m,false);
}

Matriz4x4 Matriz4x4::RotacionEje(const float angulo, const float ex, const float ey, const float ez)
{
   float c = cosf(angulo);
   float s = sinf(angulo);

   float hx = (1-c)*ex;
   float hy = (1-c)*ey;
   float hz = (1-c)*ez;

   float m[4][4] = {
         {hx*ex+c,hx*ey-s*ez,hx*ez+s*ey,0},
         {hy*ex+s*ez,hy*ey+c,hy*ez-s*ex,0},
         {hz*ex-s*ey,hz*ey+s*ex,hz*ez+c,0},
         {0,0,0,1}
   };

   return Matriz4x4(m,false);
}

Matriz4x4 Matriz4x4::RotacionEje(const float angulo, const Tupla3f eje)
{
   return Matriz4x4::RotacionEje(angulo, eje[X], eje[Y], eje[Z]);
}

/*Matriz4x4 Matriz4x4::RotacionPunto(const float angulo, const Tupla3f punto, const Tupla3f origen)
{
   Matriz4x4 traslacion_origen = Matriz4x4::Traslacion(punto[X]-origen[X], punto[Y] - origen[Y], punto[Z] - origen[Z]);
   Matriz4x4 rotacion = Matriz4x4::Rotacion
   return Matriz4x4::Traslacion()
}*/

Matriz4x4 Matriz4x4::Escalado(const float sx, const float sy, const float sz)
{
   float m[4][4] = {
         {sx,0,0,0},
         {0,sy,0,0},
         {0,0,sz,0},
         {0,0,0,1}
   };

   return Matriz4x4(m,false);
}

Matriz4x4 Matriz4x4::Escalado(const Tupla3f parametros)
{
   return Matriz4x4::Escalado(parametros[X], parametros[Y], parametros[Z]);
}

Matriz4x4 Matriz4x4::Traslacion(const float dx, const float dy, const float dz)
{
   float m[4][4] = {
         {1,0,0,dx},
         {0,1,0,dy},
         {0,0,1,dz},
         {0,0,0,1}
   };

   return Matriz4x4(m,false);
}

Matriz4x4 Matriz4x4::Traslacion(const Tupla3f parametros)
{
   return Matriz4x4::Traslacion(parametros[X], parametros[Y], parametros[Z]);
}
