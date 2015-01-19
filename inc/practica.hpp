#ifndef PRACTICA_HPP
#define PRACTICA_HPP

#include <GL/glew.h>
#include <GL/glut.h>

#include "enumerados.hpp"
#include "Camara.hpp"

using std::vector;
using std::string;

class Practica
{
public:
   virtual void Inicializar(int argc, char * argv[]) = 0;
   virtual void DibujarObjetos() = 0;
   virtual void CambioModoDibujo(enum visualizacion) = 0;
   virtual void CambioModoNormales() = 0;
   virtual void CambioColorFijo() = 0;
   virtual bool GestionarEvento(unsigned char tecla) = 0;
   virtual void Debug() = 0;
   virtual void Ayuda(vector<string> & strings_control) = 0;

   virtual void FijarCamara() = 0;
   virtual void FijarProyeccion(float ventana_tam_x, float ventana_tam_y) = 0;
   virtual void ModificaEjeXCamara(float nuevo) = 0;
   virtual void ModificaEjeYCamara(float nuevo) = 0;
   virtual void ModificarEscala(int signo) = 0;
protected:
   Practica() {};
   Practica(Practica const&);
   virtual ~Practica() {};
   void operator=(Practica const&);
};

#endif
