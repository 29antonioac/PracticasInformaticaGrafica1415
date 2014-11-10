
#ifndef TUPLAS2_HPP
#define TUPLAS2_HPP

#include <cmath>
#include <iostream>
#include <exception>
#include <vector>
#include <cassert>

// ---------------------------------------------------------------------
// índices de los ejes X,Y Z
                        
const unsigned int 
   X=0, Y=1, Z=2, W=3 ;      // indices de los ejes

// *********************************************************************
// tuplas con tres valores de tipo float
 
template <unsigned N,class T>
class Tupla
{
private:
   T coo[N];
public:
   inline Tupla<N,T>(){};
   inline Tupla( const T x, const T y, const T z );
   inline Tupla( const T x, const T y, const T z, const T w);
   inline Tupla( const T coot[N] );
   inline Tupla( const std::vector<T> & v);
   inline Tupla<N,T> operator + ( const Tupla<N,T> & t2 );
   inline Tupla<N,T>& operator+= (const Tupla<N,T> & t2) ;
   inline Tupla<N,T> operator - ( const Tupla<N,T> & t2 );
   inline Tupla<N,T>& operator-= (const Tupla<N,T> & t2) ;
   inline Tupla<N,T> operator * ( T a );
   inline Tupla<N,T> operator / ( T a );
   inline T operator | ( const Tupla<N,T> & t2 );
   inline float lenSq();
   inline float len();
   inline Tupla<N,T> normalized();


   /* Añadidos propios a la clase Tupla<N,T> */
   inline Tupla<N,T> abs();
   inline Tupla<N,T>& operator=(const Tupla<N,T> & v );
   inline T& operator[](std::size_t i);
   inline const T& operator[](std::size_t i) const;
   inline T* getPuntero();
   //template <class U>
   friend inline std::ostream& operator<<(std::ostream &f, const Tupla<N,T> &v)
   {
      f << "(";
      for (unsigned i = 0; i < N - 1; i++)
      {
         f << v[i] << ",";
      }
      f << v[N - 1] << ")" << std::flush;

      return f;
   }
};

// ---------------------------------------------------------------------
template <unsigned N,class T>
inline Tupla<N,T>::Tupla( const T x, const T y, const T z )
{
   assert (N == 3);

   coo[X] = x;
   coo[Y] = y;
   coo[Z] = z;
}

template <unsigned N, class T>
inline Tupla<N,T>::Tupla( const T x, const T y, const T z, const T w)
{
   assert (N == 4);

   coo[X] = x;
   coo[Y] = y;
   coo[Z] = z;
   coo[W] = w;
}
// ---------------------------------------------------------------------
template <unsigned N,class T>
inline Tupla<N,T>::Tupla( const T coot[N] )
{
   for (unsigned i = 0; i < N; i++)
   {
      coo[i] = coot[i];
   }
}

template <unsigned N, class T>
inline Tupla<N,T>::Tupla(const std::vector<T> & v)
{
   for (unsigned i = 0; i < N; i++)
   {
      coo[i] = v[i];
   }
}

// ---------------------------------------------------------------------
// tupla = tupla+tupla
template <unsigned N,class T>
inline Tupla<N,T> Tupla<N,T>::operator + ( const Tupla<N,T> & t2 )
{
   Tupla<N,T> suma;

   for (unsigned i = 0; i < N; i++)
   {
      suma[i] = coo[i] + t2[i];
   }
   return suma;
}

template <unsigned N,class T>
inline Tupla<N,T>& Tupla<N,T>::operator+= (const Tupla<N,T> & t2)
{
   for (unsigned i = 0; i < N; i++)
   {
      coo[i] += t2[i];
   }

   return *this;
}

// ---------------------------------------------------------------------
// tupla = tupla-tupla
template <unsigned N,class T>
inline Tupla<N,T> Tupla<N,T>::operator - ( const Tupla<N,T> & t2 )
{
   Tupla<N,T> diferencia;

   for (unsigned i = 0; i < N; i++)
   {
      diferencia[i] = coo[i] - t2[i];
   }
   return diferencia;
}

template <unsigned N,class T>
inline Tupla<N,T>& Tupla<N,T>::operator-= (const Tupla<N,T> & t2)
{
   for (unsigned i = 0; i < N; i++)
   {
      coo[i] -= t2[i];
   }

   return *this;
}

// ---------------------------------------------------------------------
// tupla3 = T*tupla3
template <unsigned N,class T>
inline Tupla<N,T> Tupla<N,T>::operator * ( T a )
{
   Tupla<N,T> producto;
   for (unsigned i = 0; i < N; i++)
   {
      producto[i] = coo[i]*a;
   }
   return producto;
}

// Definimos el operator * para aceptar también opearaciones a*v
// No hay manera de meter esto dentro de la clase luego debemos dejarlo
// fuera
template <unsigned N,class T>
inline Tupla<N,T> operator * (T a, Tupla<N,T> & v)
{
   return v*a;
}

// ---------------------------------------------------------------------
// tupla = tupla/T
template <unsigned N,class T>
inline Tupla<N,T> Tupla<N,T>::operator / ( T a )
{
   Tupla<N,T> cociente;
   for (unsigned i = 0; i < N; i++)
   {
      cociente[i] = coo[i]/a;
   }
   return cociente;
}

// ---------------------------------------------------------------------
// T = tupla|tupla (producto escalar)
template <unsigned N,class T>
inline T Tupla<N,T>::operator | ( const Tupla<N,T> & t2 )
{
   T producto_escalar = 0;
   for (unsigned i = 0; i < N; i++)
   {
      producto_escalar += coo[i]*t2[i];
   }
   return producto_escalar;
}



// ---------------------------------------------------------------------
// float = lenSq(tupla)
template <unsigned N,class T>
inline float Tupla<N,T>::lenSq( )
{
   return *this|*this;
}
// ---------------------------------------------------------------------
// float = len(tupla)
template <unsigned N,class T>
inline float Tupla<N,T>::len( )
{
   return float(sqrt(double(lenSq()))) ;
}

//----------------------------------------------------------------------
// tupla3 = normalized(tupla3)

template <unsigned N,class T>
inline Tupla<N,T> Tupla<N,T>::normalized( )
{
   return  *this/len() ;
}

template <unsigned N,class T>
inline Tupla<N,T> Tupla<N,T>::abs()
{
   Tupla<N,T> abs;
   for (unsigned i = 0; i < N; i++)
   {
      abs[i] = fabs(coo[i]);
   }
   return abs;
}

// operador de asignación

template <unsigned N,class T>
inline Tupla<N,T>& Tupla<N,T>::operator=(const Tupla<N,T> & v )
{
   if (this != &v)
   {
      for (unsigned i = 0; i < N; i++)
      {
         coo[i] = v[i];
      }
   }
   return *this;
}

// operador de indexación
template <unsigned N,class T>
T& Tupla<N,T>::operator[](std::size_t i)
{
   return coo[i];
   try
   {
      return coo[i];
   }
   catch (std::exception& e)
   {
      std::cout << "Excepción en Tupla<N,T> " << e.what() << std::endl;
   }
}

template <unsigned N,class T>
const T& Tupla<N,T>::operator[](std::size_t i) const
{
   return coo[i];
   try
   {
      return coo[i];
   }
   catch (std::exception& e)
   {
      std::cout << "Excepción en Tupla<N,T> " << e.what() << std::endl;
   }
}

template <unsigned N,class T>
T* Tupla<N,T>::getPuntero()
{
   return coo;
}
/*
// operador de salida
template <unsigned N,class T>
std::ostream& operator<<<>(std::ostream& f, const Tupla<N,T> &v)
{
   f << "(";
   for (unsigned i = 0; i < N - 1; i++)
   {
      f << v[i] << ",";
   }
   f << v[N - 1] << ")" << std::flush;

   return f;
}*/

template class Tupla<3,int>;
template class Tupla<3,float>;
template class Tupla<4,int>;
template class Tupla<4,float>;

typedef Tupla<3,float>  Tupla3f;
typedef Tupla<4,float>  Tupla4f;
typedef Tupla<3,int>    Tupla3i;




// ---------------------------------------------------------------------
//  = tupla*tupla (producto vectorial)
inline Tupla3f operator * ( const Tupla3f & t1, const Tupla3f & t2 )
{
   return Tupla3f
      (  t1[Y]*t2[Z] - t1[Z]*t2[Y] ,
         t1[Z]*t2[X] - t1[X]*t2[Z] ,
         t1[X]*t2[Y] - t1[Y]*t2[X]
      );
}


// *********************************************************************
// tuplas con cuatro valores de tipo float
 
/*
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


*/



#endif
