// *********************************************************************
// **
// ** Gestión de tuplas de coordenadas o colores 
// ** (declaraciones e implementación 'inline')
// ** Copyright (C) 2014 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// 


#ifndef TUPLAS2_HPP
#define TUPLAS2_HPP

#include <cmath>
#include <iostream>
#include <exception>

// ---------------------------------------------------------------------
// índices de los ejes X,Y Z
                        
const unsigned int 
   X=0, Y=1, Z=2, W=3 ;      // indices de los ejes

// *********************************************************************
// tuplas con tres valores de tipo float
 
/*struct Tupla3f          // tupla con 3 valores floates que representan coordenas
{                       
   float coo[3] ;       
   
   inline Tupla3f( const float x, const float y, const float z );
   inline Tupla3f( const float coot[3] );
   Tupla3f() {};
} ;*/

class Tupla3f
{
private:
   float coo[3];
public:
   inline Tupla3f( const float x, const float y, const float z );
   inline Tupla3f( const float coot[3] );
   inline Tupla3f operator + ( const Tupla3f & t2 );
   inline Tupla3f& operator+= (const Tupla3f & t2) ;
   inline Tupla3f operator - ( const Tupla3f & t2 );
   inline Tupla3f& operator-= (const Tupla3f & t2) ;
   inline Tupla3f operator * ( float a );
   inline Tupla3f operator / ( float a );
   inline float operator | ( const Tupla3f & t2 );
   inline Tupla3f operator * ( const Tupla3f & v2 );
   inline float lenSq();
   inline float len();
   inline Tupla3f normalized();
   inline Tupla3f(){};

   /* Añaidos propios a la clase Tupla3f */
   inline Tupla3f abs();
   inline Tupla3f& operator=(const Tupla3f & v );
   inline float& operator[](std::size_t i);
   inline const float& operator[](std::size_t i) const;
   inline float* getPuntero();
   friend inline std::ostream& operator<<(std::ostream &f, const Tupla3f &v);



};

// ---------------------------------------------------------------------
inline Tupla3f::Tupla3f( const float x, const float y, const float z )
{
   coo[X] = x ;
   coo[Y] = y ;
   coo[Z] = z ;
}
// ---------------------------------------------------------------------
inline Tupla3f::Tupla3f( const float coot[3] )
{
   coo[X] = coot[X] ;
   coo[Y] = coot[Y] ;
   coo[Z] = coot[Z] ;
}

// ---------------------------------------------------------------------
// tupla = tupla+tupla
inline Tupla3f Tupla3f::operator + ( const Tupla3f & t2 )
{
   return Tupla3f(coo[X]+t2.coo[X],
                  coo[Y]+t2.coo[Y],
                  coo[Z]+t2.coo[Z]);
}

inline Tupla3f& Tupla3f::operator+= (const Tupla3f & t2)
{
   coo[X]+=t2.coo[X];
   coo[Y]+=t2.coo[Y];
   coo[Z]+=t2.coo[Z];

   return *this;
}

// ---------------------------------------------------------------------
// tupla = tupla-tupla
inline Tupla3f Tupla3f::operator - ( const Tupla3f & t2 )
{
   return Tupla3f(coo[X]-t2.coo[X],
                  coo[Y]-t2.coo[Y],
                  coo[Z]-t2.coo[Z]);
}

inline Tupla3f& Tupla3f::operator-= (const Tupla3f & t2)
{
   coo[X]-=t2.coo[X];
   coo[Y]-=t2.coo[Y];
   coo[Z]-=t2.coo[Z];

   return *this;
}

// ---------------------------------------------------------------------
// tupla3 = float*tupla3
inline Tupla3f Tupla3f::operator * ( float a )
{
   return Tupla3f(a*coo[X],a*coo[Y],a*coo[Z]);
}

// Definimos el operator * para aceptar también opearaciones a*v
// No hay manera de meter esto dentro de la clase luego debemos dejarlo
// fuera
inline Tupla3f operator * (float a, Tupla3f & v)
{
   return v*a;
}

// ---------------------------------------------------------------------
// tupla = tupla/float
inline Tupla3f Tupla3f::operator / ( float a )
{
   return Tupla3f( coo[X]/a, coo[Y]/a, coo[Z]/a );
}

// ---------------------------------------------------------------------
// float = tupla|tupla (producto escalar)
inline float Tupla3f::operator | ( const Tupla3f & t2 )
{
   return   coo[X]*t2.coo[X] +
            coo[Y]*t2.coo[Y] +
            coo[Z]*t2.coo[Z] ;
}

// ---------------------------------------------------------------------
//  = tupla*tupla (producto vectorial)
inline Tupla3f Tupla3f::operator * ( const Tupla3f & v2 )
{
   return Tupla3f
         (  coo[Y]*v2.coo[Z] - coo[Z]*v2.coo[Y] ,
            coo[Z]*v2.coo[X] - coo[X]*v2.coo[Z] ,
            coo[X]*v2.coo[Y] - coo[Y]*v2.coo[X]
         );
}

// ---------------------------------------------------------------------
// float = lenSq(tupla)
inline float Tupla3f::lenSq( )
{
   return *this|*this;
}
// ---------------------------------------------------------------------
// float = len(tupla)
inline float Tupla3f::len( )
{
   return float(sqrt(double(lenSq()))) ;
}

//----------------------------------------------------------------------
// tupla3 = normalized(tupla3)

inline Tupla3f Tupla3f::normalized( )
{
   return  *this/len() ;
}

inline Tupla3f Tupla3f::abs()
{
   Tupla3f abs;
   for (unsigned i = 0; i < 3; i++)
   {
      abs[i] = fabs(coo[i]);
   }
   return abs;
}

// operador de asignación

inline Tupla3f& Tupla3f::operator=(const Tupla3f & v )
{
   if (this == &v)
      return *this;
   coo[X] = v.coo[X];
   coo[Y] = v.coo[Y];
   coo[Z] = v.coo[Z];
   return *this;
}

// operador de indexación
float& Tupla3f::operator[](std::size_t i)
{
   return coo[i];
   try
   {
      return coo[i];
   }
   catch (std::exception& e)
   {
      std::cout << "Excepción en Tupla3f " << e.what() << std::endl;
   }
}


const float& Tupla3f::operator[](std::size_t i) const
{
   return coo[i];
   try
   {
      return coo[i];
   }
   catch (std::exception& e)
   {
      std::cout << "Excepción en Tupla3f " << e.what() << std::endl;
   }
}

float* Tupla3f::getPuntero()
{
   return coo;
}

// operador de salida

std::ostream& operator<<(std::ostream& f, const Tupla3f &v)
{
   f << "(" << v.coo[X] << "," << v.coo[Y] << "," << v.coo[Z] << ")" << std::flush;
   return f;
}

// *********************************************************************
// tuplas con cuatro valores de tipo float
 
class Tupla4f          // tupla con 3 valores floates que representan coordenas
{
private:
   float coo[4] ;       
public:
   inline Tupla4f( const float x, const float y, const float z, const float w );
   inline Tupla4f( const float coot[4] );
   inline float& operator[](std::size_t i);
   inline const float& operator[](std::size_t i) const;
   Tupla4f() {};
} ;

// ---------------------------------------------------------------------
inline Tupla4f::Tupla4f( const float x, const float y, const float z, const float w  )
{
   coo[X] = x ;
   coo[Y] = y ;
   coo[Z] = z ;
   coo[W] = w ;
}
// ---------------------------------------------------------------------
inline Tupla4f::Tupla4f( const float coot[4] )
{
   coo[X] = coot[X] ;
   coo[Y] = coot[Y] ;
   coo[Z] = coot[Z] ;
   coo[W] = coot[W] ;
}

// operador de indexación
float& Tupla4f::operator[](std::size_t i)
{
   return coo[i];
   try
   {
      return coo[i];
   }
   catch (std::exception& e)
   {
      std::cout << "Excepción en Tupla3f " << e.what() << std::endl;
   }
}


const float& Tupla4f::operator[](std::size_t i) const
{
   return coo[i];
   try
   {
      return coo[i];
   }
   catch (std::exception& e)
   {
      std::cout << "Excepción en Tupla3f " << e.what() << std::endl;
   }
}

// *********************************************************************
// tuplas con tres valores int

/*struct Tupla3i
{
   int idx[3] ;
} ;*/

class Tupla3i
{
private:
   int idx[3];
public:
   inline Tupla3i() {};
   inline Tupla3i( const unsigned x, const unsigned y, const unsigned z );
   inline Tupla3i( const unsigned coot[3] );
   inline Tupla3i& operator=(const Tupla3i & v );
   inline int& operator[](std::size_t i);
   inline const int& operator[](std::size_t i) const;
   inline int* getPuntero();
   friend inline std::ostream& operator<<(std::ostream &f, const Tupla3i &v);
};

inline Tupla3i::Tupla3i( const unsigned x, const unsigned y, const unsigned z )
{
   idx[X] = x ;
   idx[Y] = y ;
   idx[Z] = z ;
}
// ---------------------------------------------------------------------
inline Tupla3i::Tupla3i( const unsigned coot[3] )
{
   idx[X] = coot[X] ;
   idx[Y] = coot[Y] ;
   idx[Z] = coot[Z] ;
}

// operador de asignación

inline Tupla3i& Tupla3i::operator=(const Tupla3i & v )
{
   if (this == &v)
      return *this;
   idx[X] = v.idx[X];
   idx[Y] = v.idx[Y];
   idx[Z] = v.idx[Z];
   return *this;
}

// operador de indexación
int& Tupla3i::operator[](std::size_t i)
{
   return idx[i];
   try
   {
      return idx[i];
   }
   catch (std::exception& e)
   {
      std::cout << "Excepción en Tupla3f " << e.what() << std::endl;
   }
}


const int& Tupla3i::operator[](std::size_t i) const
{
   return idx[i];
   try
   {
      return idx[i];
   }
   catch (std::exception& e)
   {
      std::cout << "Excepción en Tupla3f " << e.what() << std::endl;
   }
}

int* Tupla3i::getPuntero()
{
   return idx;
}

// operador de salida

std::ostream& operator<<(std::ostream& f, const Tupla3i &v)
{
   f << "(" << v.idx[X] << "," << v.idx[Y] << "," << v.idx[Z] << ")" << std::flush;
   return f;
}






#endif
