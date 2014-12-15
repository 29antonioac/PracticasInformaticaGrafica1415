#ifndef FUENTELUZ_HPP
#define FUENTELUZ_HPP

#include "tuplas.hpp"

using std::vector;

class FuenteLuz
{
protected:
   static unsigned numero_fuentes;
   int id_luz;
   Tupla3f componente_ambiental, componente_difusa, componente_especular;

public:
   FuenteLuz(Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular);
   virtual void Activar() = 0;
   virtual ~FuenteLuz() {};

};

class FuenteLuzPosicional : virtual public FuenteLuz
{
protected:
   Tupla3f posicion;
public:
   FuenteLuzPosicional(Tupla3f posicion, Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular);
   void Activar();
   Tupla3f getPos();
};

class FuenteLuzDireccional : virtual public FuenteLuz
{
protected:
   float alpha, beta;

public:
   FuenteLuzDireccional(float alpha, float beta, Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular);
   void Activar();
};

class ColeccionFuentesLuz
{
   vector<FuenteLuz *> fuentes;
   void Activar();
   void Agregar(FuenteLuz * fuente);
};


#endif
