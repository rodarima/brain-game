#Purple brain
2014O20

##Descripción
Imagínese que se encuentra en el interior de su mente. Allí puede encontrar un
montón de diminutas neuronas que se conectan entre sí. De hecho hay tantas y
tantas conexiones, que le resultaría muy complicado tratar de adivinar que
demonios es lo que está pasando.

Dentro del juego, dispone de neuronas, que podrá colocar donde desee. Y realizar
las conexiones entre las mismas y observar que sucede. Podrá controlar el
tiempo, y acercarse o alejarse a alguna zona en concreto.

##Objetivo
Por ahora, Brain no tiene ningún objetivo particular, más que la exploración de
las neuronas y sus comportamientos. 

##Funcionamiento
Una neurona puede encontrarse en 3 estados: reposo, activación e inversión.
Estos tres estados se representan mediante los colores blanco, verde y rojo.

Cuando crea una neurona, esta se encuentra en reposo. 

Sin embargo, si hace clic encima de ella, esta se activa, pasando al estado de
activación. Esto significa que producirá una salida de +1 a todas las conexiones
salientes.

Poco despues deja de estar activada, y pasa al estado de inversión, donde su
valor será de -1 para todas las conexiones salientes.

Finalmente regresará al estado de reposo. Y las conexiones salientes ahora
tomarán el valor 0.

Además de poder activar una neurona empleando el ratón, es posoble que se active
si las conexiones entrantes estan activadas de una cierta manera.

Para que se active una neurona, la suma de los valores de las conexiones
entrantes debe superar un umbral. Este umbral se calcula para cada neurona.
Consiste en sumar el número de neuronas diferentes de entrada, y dividir dicho
número entre dos. Redondeando hacia arriba en caso de ser numero impar.

Pongamos un ejemplo. Una neurona tiene 3 conexiones de entrada, que proceden de
3 neuronas diferentes. El umbral para esa neurona es 3/2=  1.5, redondeado a 2.

Por lo tanto, para que se active necesita un nivel de +2 o superior.

En el ejemplo anterior si 2 neuronas están activadas y la tercera en reposo,
tendremos la entrada +1, +1, 0, que suma +2, por lo tanto se activará.

##Niveles
Para abrir un nivel ya guardado, añada la ruta a dicho archivo a la línea de
comando.

Podrá guardar el estado de una partida pulsando s. Es importante haber
especificado la ruta de antemano.

##Controles
El juego está pensado para ser jugado empleando un ratón únicamente. Es
necesario que disponga de una rueda.

Pulse el botón izquierdo sobre una zona vacía para crear neuronas, o encima de
una para activarla. Pulse y arrastre para unir dos neuronas.

Emplee el botón derecho para borrar neuronas, o arrastre para moverlas.

La rueda controla el zoom y la velocidad del juego, que es inherente a lo cerca
o lejos que se encuentre de la escena.

##Instalación
Descarge el código del siguiente enlace:
[Purple brain en github](https://github.com/rodarima/purple-brain)
Debe disponer de un sistema GNU/Linux, e instalar los paquetes de openGL y
freeglut.

Ejecute solamente:

	$ make

Y podra probar el juego con:

	$ ./brain level/1

Diviértase!
