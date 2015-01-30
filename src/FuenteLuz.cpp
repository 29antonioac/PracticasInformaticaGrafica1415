#include "FuenteLuz.hpp"

#include <GL/gl.h>
#include <iostream>

unsigned FuenteLuz::numero_fuentes = 0;


FuenteLuz::FuenteLuz(Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular)
{
   this->componente_ambiental = AniadeW(componente_ambiental,1.0);
   this->componente_difusa = AniadeW(componente_difusa,1.0);
   this->componente_especular = AniadeW(componente_especular,1.0);
   this->id_luz = GL_LIGHT0 + numero_fuentes;
   numero_fuentes++;
}


void FuenteLuz::Desactivar()
{
   glDisable(this->id_luz);
}

void FuenteLuz::DibujarFuente()
{
   /*
   glDisable( GL_LIGHTING );
   glPointSize(60.0f);
   glColor3f(0.0f,0.0f,1.0f);
   glBegin(GL_POINTS);
   glVertex4fv(this->getPosVec().data());
   glEnd();
   glEnable( GL_LIGHTING );
   */
}

Tupla4f FuenteLuzPosicional::getPosVec()
{
   return this->posicion;
}

Tupla4f FuenteLuzDireccional::getPosVec()
{
   Tupla4f res(cosf(alpha)*sinf(beta),sinf(alpha)*sinf(beta),cosf(beta),1.0);
   return res * 100;
}


FuenteLuzPosicional::FuenteLuzPosicional(Tupla3f posicion, Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular)
   : FuenteLuz::FuenteLuz(componente_ambiental, componente_difusa, componente_especular)
{
   this->posicion = AniadeW(posicion,1.0f);
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

   DibujarFuente();

   Tupla4f ejeZ(cosf(alpha)*sinf(beta),sinf(alpha)*sinf(beta),cosf(beta),0.0);

   glMatrixMode(GL_MODELVIEW);

   glLightfv(id_luz, GL_POSITION, ejeZ.data());
   glLightfv(id_luz, GL_AMBIENT, this->componente_ambiental.data());
   glLightfv(id_luz, GL_DIFFUSE, this->componente_difusa.data());
   glLightfv(id_luz, GL_SPECULAR,this->componente_especular.data());
}

void FuenteLuzDireccional::ModificaAlpha(int signo)
{
	assert(signo == 1 || signo == -1);
	if (alpha > 2*M_PI) alpha -= 2*M_PI;
	else if (alpha < 0) alpha += 2*M_PI;
	alpha += signo*M_PI/30;
}

void FuenteLuzDireccional::ModificaBeta(int signo)
{
	assert(signo == 1 || signo == -1);
   if (beta > 2*M_PI) beta -= 2*M_PI;
   else if (beta < 0) beta += 2*M_PI;
	beta += signo*M_PI/30;
}

float FuenteLuzDireccional::getAlpha()
{
   return alpha;
}
float FuenteLuzDireccional::getBeta()
{
   return beta;
}

void FuenteLuzPosicional::Activar()
{
   glEnable(id_luz);

   DibujarFuente();

   glMatrixMode(GL_MODELVIEW);

   glLightfv(id_luz, GL_POSITION,this->posicion.data());
   glLightfv(id_luz, GL_AMBIENT, this->componente_ambiental.data());
   glLightfv(id_luz, GL_DIFFUSE, this->componente_difusa.data());
   glLightfv(id_luz, GL_SPECULAR,this->componente_especular.data());

}

void ColeccionFuentesLuz::Activar()
{

   for (unsigned i = 0; i < fuentes.size(); i++)
   {
      fuentes[i]->Activar();
   }
}

void ColeccionFuentesLuz::Desactivar()
{
   for (unsigned i = 0; i < fuentes.size(); i++)
   {
      fuentes[i]->Desactivar();
   }
}

void ColeccionFuentesLuz::Agregar(FuenteLuz * fuente)
{
   if (fuentes.size() < 8)
      fuentes.push_back(fuente);
   else
      std::cout << "Ya hay 8 fuentes, que son las máximas!" << std::endl;
}




