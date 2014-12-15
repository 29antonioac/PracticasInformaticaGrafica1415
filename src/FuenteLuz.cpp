#include "FuenteLuz.hpp"

#include <GL/gl.h>

unsigned FuenteLuz::numero_fuentes = 0;

FuenteLuz::FuenteLuz(Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular)
{
   this->componente_ambiental = componente_ambiental;
   this->componente_difusa = componente_difusa;
   this->componente_especular = componente_especular;
   this->id_luz = GL_LIGHT0 + numero_fuentes;
   numero_fuentes++;
}

FuenteLuzPosicional::FuenteLuzPosicional(Tupla3f posicion, Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular)
   : FuenteLuz::FuenteLuz(componente_ambiental, componente_difusa, componente_especular)
{
   this->posicion = posicion;
}

FuenteLuzDireccional::FuenteLuzDireccional(float alpha, float beta, Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular)
   : FuenteLuz::FuenteLuz(componente_ambiental, componente_difusa, componente_especular)
{
   this->alpha = alpha;
   this->beta = beta;
}

void FuenteLuzDireccional::Activar()
{
   glEnable(id_luz);
}

void FuenteLuzPosicional::Activar()
{
   glEnable(id_luz);
   glLightfv(id_luz, GL_POSITION,this->posicion.data());
   glLightfv(id_luz, GL_AMBIENT, this->componente_ambiental.data());
   glLightfv(id_luz, GL_DIFFUSE, this->componente_difusa.data());
   glLightfv(id_luz, GL_SPECULAR,this->componente_especular.data());
}

Tupla3f FuenteLuzPosicional::getPos()
{
   return posicion;
}

void ColeccionFuentesLuz::Activar()
{
   for (unsigned i = 0; i < fuentes.size(); i++)
   {
      glEnable(GL_LIGHT0 + i);
      //fuentes[i]->Activar();
      //glLightfv(GL_LIGHT0 + i, GL_POSITION, fuentes[i]->getPos());
   }
}

void ColeccionFuentesLuz::Agregar(FuenteLuz * fuente)
{
   if (fuentes.size() < 8)
      fuentes.push_back(fuente);
   else
      std::cout << "Ya hay 8 fuentes, que son las máximas!" << std::endl;
}
