Prácticas de Informática Gráfica 14-15
===============================

Prácticas de Informática Gráfica 14-15 de la Universidad de Granada. Su objetivo es familiarizar
a los alumnos con el desarrollo de aplicaciones con freeglut.

Las prácticas de esta asignatura son incrementales luego todas ellas irán a este mismo proyecto.
Pulsando Fx se va cambiando de práctica.

Para ejecutar se usa 

    bin/prac \<nombre_del_ply.ply\> \<nombre_del_perfil.ply\> \<numero_de_caras\> \<modo_practica_2\> 

O se especifica todo o cargará ant.ply, arboldenavidad.ply, 20 y 0. Los ply están en la carpeta PLY. 
  El modo práctica 2 es
- 0 revolución sobre el eje Y (usar arboldenavidad.ply)
- 1 barrido por rotación sobre el eje Y (usar toro.ply)
- 2 barrido por traslación de un perfil (usar prisma.ply)

Para compilar se usa

    make

Como el make de la web está preparado para autoejecutar la práctica, se puede usar

    make PLY=nombre_del_ply.ply PERFIL=nombre_del_perfil.ply N=numero_de_caras
  
para cargar insertar argumentos distintos a los por defecto del programa.

Dentro del entorno gráfico de la práctica se pueden usar los siguientes botones, a parte de los ya proporcionados:
- **L** para dibujar en modo alambre (este es el modo por defecto)
- **S** para dibujar en modo sólido
- **D** para dibujar en modo sólido con atributos de caras
- **A** para dibujar en modo ajedrez
- **P** para dibujar en modo punto
- **H** para dibujar normales de caras, vértices o ambas
- **R** para resetear la cámara
  
Para mover la cámara se puede usar el ratón, para ello hay que clicar en la ventana con el botón derecho y moverlo
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

## Práctica 3

Esta práctica consta de dibujar modelos jerárquicos usando un grafo de escena parametrizados.
Como extensión se propuso y completado una animación del modelo.

Las teclas únicas para esta pŕactica son:

- **Z/z** para modificar rotación del cuerpo
- **X/x** para modificar rotación de los brazos
- **C/c** para modificar rotación de las piernas
- **V/v** para modificar distancia al eje Y
- **B/b** para modificar velocidad de rotación del cuerpo
- **N/n** para modificar velocidad de rotación de los brazos
- **M/m** para modificar velocidad de rotación de las piernas

Esta práctica tiene un material difuso-especular para el Android, y 4 fuentes de luz posicionales en 

    (-10,0,-10) | (10,0,-10)
    ------------------------
    (-10,0,10)  | (10,0,10)

como extensión de la práctica 4.

## Práctica 4

Esta práctica consta de introducir materiales y luz a la escena.
Como extensión se propone y completado la inserción de materiales y luz para la práctica 3.

Las teclas de esta práctica son:

- **C/V** para modificar alpha (ángulo con eje Y) de la luz direccional
- **Z/X** para modificar beta (ángulo con eje -X)

## Práctica 5

Esta práctica consta de interaccionar con el entorno. Partiendo de 3 cámaras (dos con proyección perspectiva y otra con proyección ortogonal) en primera persona, podremos movernos usando **W**,**A**,**S**,**D** y mover la cámara con el ratón, al igual que en las demás prácticas. 

Podemos clickar en una de las mallas que componen la escena y la cámara entrará en modo examinar, fijando la vista en el centro de la malla clickada. La selección de figura se hace por selección de colores en el buffer trasero. 

## Común

Esta versión de las prácticas con cauce fijo tiene disponible con F6 y F7 un menú de debug y de ayuda respectivamente.
Aconsejo probarlo, aunque puede ralentizar el renderizado en según qué GPU's.



