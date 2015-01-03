#ifndef FUENTELUZ_HPP
#define FUENTELUZ_HPP

#include <vector>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using std::vector;
using std::cout;

class FuenteLuz
{
protected:
   static unsigned numero_fuentes;
   int id_luz;
   glm::vec4 componente_ambiental, componente_difusa, componente_especular;

public:
   FuenteLuz(glm::vec3 componente_ambiental, glm::vec3 componente_difusa, glm::vec3 componente_especular);
   virtual void Activar() = 0;
   virtual ~FuenteLuz() {};
   static unsigned getFuentes() { return numero_fuentes; }

};

class FuenteLuzPosicional : virtual public FuenteLuz
{
private:
   glm::vec4 posicion;
public:
   FuenteLuzPosicional(glm::vec3 posicion, glm::vec3 componente_ambiental, glm::vec3 componente_difusa, glm::vec3 componente_especular);
   void Activar();
};

class FuenteLuzDireccional : virtual public FuenteLuz
{
private:
   float alpha, beta;
public:
   FuenteLuzDireccional(float alpha, float beta, glm::vec3 componente_ambiental, glm::vec3 componente_difusa, glm::vec3 componente_especular);
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
   void Agregar(FuenteLuz * fuente);
};


#endif
