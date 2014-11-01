#ifndef __IG_VBO_HPP__
#define __IG_VBO_HPP__

class VBO
{
private:
   GLuint id_vbo;
   GLuint tipo;
   unsigned tamanio;
   GLvoid * datos;
public:
   VBO( GLuint tipo, GLuint tamanio, GLvoid * puntero );
   //void VisualizarVBO();
   GLuint getID();
   GLvoid * getDatos();



};



#endif
