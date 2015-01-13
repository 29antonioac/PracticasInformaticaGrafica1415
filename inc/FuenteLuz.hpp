#ifndef FUENTELUZ_HPP
#define FUENTELUZ_HPP

#include "tuplas.hpp"

using std::vector;
using std::cout;

class FuenteLuz
{
protected:
   static unsigned numero_fuentes;
   int id_luz;
   Tupla4f componente_ambiental, componente_difusa, componente_especular;

public:
   FuenteLuz(Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular);
   virtual void Activar() = 0;
   void Desactivar();
   virtual ~FuenteLuz() {};
   static unsigned getFuentes() { return numero_fuentes; }

};

class FuenteLuzPosicional : virtual public FuenteLuz
{
private:
   Tupla4f posicion;
public:
   FuenteLuzPosicional(Tupla3f posicion, Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular);
   void Activar();
};

class FuenteLuzDireccional : virtual public FuenteLuz
{
private:
   float alpha, beta;
public:
   FuenteLuzDireccional(float alpha, float beta, Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular);
   void Activar();
   float getAlpha();
   float getBeta();
   void ModificaAlpha(int signo);
   void ModificaBeta(int signo);
};

class ColeccionFuentesLuz
{
private:
   vector<FuenteLuz *> fuentes;
public:
   void Activar();
   void Desactivar();
   void Agregar(FuenteLuz * fuente);
};


#endif
