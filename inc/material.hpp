#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "textura.hpp"

class Material
{
private:
   Textura * textura;
   Tupla3f componente_emision, componente_ambiental, componente_difusa, componente_especular;
   float exponente_especular;

public:
   void Activar();
};



#endif
