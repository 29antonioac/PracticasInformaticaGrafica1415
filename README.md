Prácticas de Informática Gráfica 14-15
===============================

Prácticas de Informática Gráfica 14-15 de la Universidad de Granada. Su objetivo es familiarizar
a los alumnos con el desarrollo de aplicaciones con freeglut.

Las prácticas de esta asignatura son incrementales luego todas ellas irán a este mismo proyecto.
Pulsando Fx se va cambiando de práctica. Por ahora las disponibles son:

## Práctica 1

Esta práctica consta de dibujar modelos usando un fichero ply.

- Para ejecutar se usa **./prac \<nombre_del_ply.ply\>**
  Si no se especifica un ply a cargar se cargará ant.ply

- Para compilar se usa **make**. Como el make de la web está preparado para autoejecutar
  la práctica, se puede usar **make PLY=nombre_del_ply.ply** para cargar un ply distinto
  a ant.ply, que como comento es el que carga por defecto en el programa.

Dentro del entorno gráfico de la práctica se pueden usar los siguientes botones, a parte de los ya proporcionados:
- **A** para dibujar en modo alambre (este es el modo por defecto)
- **S** para dibujar en modo sólido
- **C** para dibujar en modo ajedrez
- **P** para dibujar en modo punto
- **R** para resetear la cámara
  
Para mover la cámara se puede usar el ratón, para ello hay que clicar en la ventana y moverlo
en la dirección deseada. También se puede usar el scroll para aumentar o disminuir el zoom
y, si están disponibles, los botones extra del ratón se pueden usar para girar también la cámara
a ambos lados. En mi caso dichos botones son desplazar la rueda del ratón a izquierda y derecha.

