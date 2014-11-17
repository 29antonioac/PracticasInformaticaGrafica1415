Prácticas de Informática Gráfica 14-15
===============================

Prácticas de Informática Gráfica 14-15 de la Universidad de Granada. Su objetivo es familiarizar
a los alumnos con el desarrollo de aplicaciones con freeglut.

Las prácticas de esta asignatura son incrementales luego todas ellas irán a este mismo proyecto.
Pulsando Fx se va cambiando de práctica.

- Para ejecutar se usa **bin/prac \<nombre_del_ply.ply\> \<nombre_del_perfil.ply\> \<numero_de_caras\>**
  O se especifica todo o cargará ant.ply. Como perfil y número de caras cargará el que dictaminen las directivas
  de preprocesador de practica2.cpp

- Para compilar se usa **make**. Como el make de la web está preparado para autoejecutar
  la práctica, se puede usar **make PLY=nombre_del_ply.ply PERFIL=nombre_del_perfil.ply N=numero_de_caras** 
  para cargar insertar argumentos distintos a los por defecto del programa.

Dentro del entorno gráfico de la práctica se pueden usar los siguientes botones, a parte de los ya proporcionados:
- **L** para dibujar en modo alambre (este es el modo por defecto)
- **S** para dibujar en modo sólido
- **D** para dibujar en modo sólido con atributos de caras
- **A** para dibujar en modo ajedrez
- **P** para dibujar en modo punto
- **R** para resetear la cámara
  
Para mover la cámara se puede usar el ratón, para ello hay que clicar en la ventana y moverlo
en la dirección deseada. También se puede usar el scroll para aumentar o disminuir el zoom
y, si están disponibles, los botones extra del ratón se pueden usar para girar también la cámara
a ambos lados. En mi caso dichos botones son desplazar la rueda del ratón a izquierda y derecha.

## Práctica 1

Esta práctica consta de dibujar modelos usando un fichero ply.

Simplemente se crea una malla usando el ply cargado y se muestra utilizando VBO (Vertex Buffered Objects).
Se debe poder alternar entre modo punto, sólido, alambre y ajedrez.

## Práctica 2

Esta práctica consta de modelar objetos por revolución y aplicar cálculos geométricos a las mallas.

Para ello, partiendo de un perfil en el plano **Z = 0** se aplica una matriz de rotación sobre sus vértices
para conseguir un modelo completo.

Como extensión se propuso y se ha completado el modelado por barrido, tanto por rotación como por traslación de un
contorno cerrado.



