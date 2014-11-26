#ifndef PRACTICA_HPP
#define PRACTICA_HPP

class Practica
{
public:
   virtual void Inicializar(int argc, char * argv[]) = 0;
   virtual void DibujarObjetos() = 0;
   virtual void CambioModoDibujo(enum visualizacion) = 0;
   virtual void CambioModoNormales() = 0;
protected:
   Practica() {};
   Practica(Practica const&);
   virtual ~Practica() {};
   void operator=(Practica const&);
};

#endif
