#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "textura.hpp"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Material
{
private:
   Textura * textura;
   glm::vec4 componente_emision, componente_ambiental, componente_difusa, componente_especular;
   float exponente_especular;

public:
   Material(glm::vec3 componente_emision, glm::vec3 componente_ambiental, glm::vec3 componente_difusa,
         glm::vec3 componente_especular, float exponente_especular, Textura * textura = nullptr);
   bool Activar();
   bool NecesitoCoordenadasTextura();
};



#endif
