#ifndef TUPLAS_HPP
#define TUPLAS_HPP

#include <cmath>
#include <iostream>
#include <exception>
#include <vector>
#include <cassert>

using std::vector;
using std::ostream;
using std::flush;
using std::cout;
using std::endl;
using std::exception;

// ---------------------------------------------------------------------
// índices de los ejes X,Y Z
                        
const unsigned int 
   X = 0, Y = 1, Z = 2, W = 3 ;      // indices de los ejes
 
template <unsigned N,class T>
class Tupla
{
private:
   T coo[N];
public:
   inline Tupla<N,T>();
   inline Tupla( const T x, const T y, const T z );
   inline Tupla( const T x, const T y, const T z, const T w);
   inline Tupla( const T coot[N] );
   inline Tupla( const vector<T> & v);
   inline Tupla<N,T> operator + ( const Tupla<N,T> & t2 );
   inline Tupla<N,T>& operator+= (const Tupla<N,T> & t2) ;
   inline Tupla<N,T> operator - ( const Tupla<N,T> & t2 );
   inline Tupla<N,T>& operator-= (const Tupla<N,T> & t2) ;
   inline Tupla<N,T> operator + (T a);
   inline Tupla<N,T> operator - (T a);
   inline Tupla<N,T> operator * ( T a );
   inline Tupla<N,T> operator / ( T a );
   inline T operator | ( const Tupla<N,T> & t2 );
   inline float lenSq();
   inline float len();
   inline Tupla<N,T> normalized();


   inline Tupla<N,T> abs();
   inline Tupla<N,T>& operator=(const Tupla<N,T> & v );
   inline T& operator[](size_t i);
   inline const T& operator[](size_t i) const;
   inline T* data();

   // Operador de flujo de salida
   friend inline ostream& operator<<(ostream &flujo, const Tupla<N,T> &t)
   {
      flujo << "(";
      for (unsigned i = 0; i < N - 1; i++)
      {
         flujo << t[i] << ",";
      }
      flujo << t[N - 1] << ")" << flush;

      return flujo;
   }
};

template <unsigned N, class T>
inline Tupla<N,T>::Tupla()
{
   for (unsigned i = 0; i < N; i++)
      coo[i] = 0;
}

// Constructor de tupla con 3 elementos
template <unsigned N,class T>
inline Tupla<N,T>::Tupla( const T x, const T y, const T z )
{
   assert (N == 3);

   coo[X] = x;
   coo[Y] = y;
   coo[Z] = z;
}

// Constructor de tupla con 4 elementos
template <unsigned N, class T>
inline Tupla<N,T>::Tupla( const T x, const T y, const T z, const T w)
{
   assert (N == 4);

   coo[X] = x;
   coo[Y] = y;
   coo[Z] = z;
   coo[W] = w;
}

// Constructor de copia usando un array
template <unsigned N,class T>
inline Tupla<N,T>::Tupla( const T v[N] )
{
   for (unsigned i = 0; i < N; i++)
   {
      coo[i] = v[i];
   }
}

// Constructor de tupla usando un std::vector
template <unsigned N, class T>
inline Tupla<N,T>::Tupla(const vector<T> & v)
{
   for (unsigned i = 0; i < N; i++)
   {
      coo[i] = v[i];
   }
}

// Operador de suma de tuplas
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

// Operador de suma con asignación
template <unsigned N,class T>
inline Tupla<N,T>& Tupla<N,T>::operator+= (const Tupla<N,T> & t2)
{
   for (unsigned i = 0; i < N; i++)
   {
      coo[i] += t2[i];
   }

   return *this;
}

// Operador de diferencia de tuplas
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

// Operador de diferencia con asignación
template <unsigned N,class T>
inline Tupla<N,T>& Tupla<N,T>::operator-= (const Tupla<N,T> & t2)
{
   for (unsigned i = 0; i < N; i++)
   {
      coo[i] -= t2[i];
   }

   return *this;
}
template <unsigned N,class T>
inline Tupla<N,T> Tupla<N,T>::operator + (T a)
{
   Tupla<N,T> suma;
   for (unsigned i = 0; i < N; i++)
   {
      suma[i] = coo[i]+a;
   }
   return suma;
}

template <unsigned N,class T>
inline Tupla<N,T> Tupla<N,T>::operator - (T a)
{
   Tupla<N,T> diferencia;
   for (unsigned i = 0; i < N; i++)
   {
      diferencia[i] = coo[i]-a;
   }
   return diferencia;
}
// Producto de tupla por escalar
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

// Cociente de tupla por escalar
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

// Producto escalar de dos tuplas
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



// Norma al cuadrado de una tupla
template <unsigned N,class T>
inline float Tupla<N,T>::lenSq( )
{
   return *this|*this;
}

// Norma de una tupla
template <unsigned N,class T>
inline float Tupla<N,T>::len( )
{
   return float(sqrt(double(lenSq()))) ;
}

// Normalización de una tupla
template <unsigned N,class T>
inline Tupla<N,T> Tupla<N,T>::normalized( )
{
   return  *this/len() ;
}

// Valor absoluto de una tupla
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

// Operador de asignación

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

// Operador de indexación
template <unsigned N,class T>
T& Tupla<N,T>::operator[](size_t i)
{
   return coo[i];
   try
   {
      return coo[i];
   }
   catch (exception& e)
   {
      cout << "Excepción en Tupla<N,T> " << e.what() << endl;
   }
}

template <unsigned N,class T>
const T& Tupla<N,T>::operator[](size_t i) const
{
   return coo[i];
   try
   {
      return coo[i];
   }
   catch (exception& e)
   {
      cout << "Excepción en Tupla<N,T> " << e.what() << endl;
   }
}

// Obtener puntero al primer elemento
template <unsigned N,class T>
T* Tupla<N,T>::data()
{
   return coo;
}

// Especializaciones
template class Tupla<3,int>;
template class Tupla<3,float>;
template class Tupla<4,float>;

typedef Tupla<3,float>  Tupla3f;
typedef Tupla<4,float>  Tupla4f;
typedef Tupla<3,int>    Tupla3i;




// Producto vectorial de dos tuplas de 3 float
inline Tupla3f operator * ( const Tupla3f & t1, const Tupla3f & t2 )
{
   return Tupla3f
      (  t1[Y]*t2[Z] - t1[Z]*t2[Y] ,
         t1[Z]*t2[X] - t1[X]*t2[Z] ,
         t1[X]*t2[Y] - t1[Y]*t2[X]
      );
}

template <unsigned N, class T>
inline Tupla<N+1,T> AniadeW(const Tupla<N,T> t, const float w)
{
   Tupla<N+1,T> resultado;
   assert(w == 1.0 || w == 0.0);

   for (unsigned i = 0; i < N; i++)
      resultado[i] = t[i];
   resultado[N] = w;

   return resultado;
}


#endif
