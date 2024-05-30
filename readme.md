Proyecto: Alien Invaders

Integrantes:
Jose Miguel Leyva
Kevin Marquez

-------------------------------------------------------------
La idea es tener 3 clases: Nave, Alien y Disparos

Cada objeto tiene unas coordenadas x,y que es lo que permite ubicarlos en la pantalla

La nave del jugador tiene que moverse y disparar(ya la parte de moverse pincha)

Para los Aliens la idea es generar oleadas de aliens aleatorias que vayan saliendo cada cierto tiempo linea x linea
Algo asi:

 \ /     \ /    \ /
(x_x)   (x_x)  (x_x)
 \ /      \ /
(x_x)    (x_x)
      \ /
     (x_x)

Cuando el jugador dispara se genera un disparo desde su posicion(hay que ponerle un cooldown de 1s pa que no se hagan 15 mil disparos si se deja apretado el espacio)

Para chequear colisiones de los disparos con los aliens se revisan sus coordenadas y si coinciden es que le diste

En ese caso se elimina el disparo y el alien(a menos que le pongamos vida y aguanten mas de un disparo)

Igualmente si un Alien toca al jugador se pierde la partida.

En la carpeta modules van a ir todas las librerias que tengamos que crear para el juego
De momento solo se me ocurren las de los objetos y la de los hilos.

El archivo con main deberia ir afuera, pero para no pasar trabajo ejecutando las cosas lo dejo dentro de momento.

Cada evento del juego seria un hilo, de momento vamos a verlo como metodos y ya al final es que creamos los hilos y hacemos la
planificacion pa que esa talla quede sincronizada.

-----------------------------------------------------------------------------------------------------
TALLAS POR HACER:

clase Alien
clase Disparo

Hilos:
CrearDisparo()
MoverDisparo() 
EliminarDisparo()


GenerarHordaAliens()
MoverHorda()
EliminarAlien()

ChequearColisiones()
UpdateScore()

