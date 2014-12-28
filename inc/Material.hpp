#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "textura.hpp"
#include "tuplas.hpp"

class Material
{
private:
   Textura * textura;
   Tupla4f componente_emision, componente_ambiental, componente_difusa, componente_especular;
   float exponente_especular;

public:
   Material(Tupla3f componente_emision, Tupla3f componente_ambiental, Tupla3f componente_difusa,
         Tupla3f componente_especular, float exponente_especular, Textura * textura = nullptr);
   bool Activar();
   bool HayTextura();
};



#endif
