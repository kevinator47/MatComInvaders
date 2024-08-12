Proyecto: Alien Invaders

Integrantes:
Jose Miguel Leyva
Kevin Marquez

El proyecto esta estructurado de la siguiente forma:

main.c : contiene la funcion principal y los hilos que ejecutan los distintos eventos del juego
modules/
gameObjects.h : contiene las definiciones de todos los elementos del juego, sus comportamientos y estructuras de datos que los almacenan
utils.h : funciones de proposito general

----------------------------------------------------------------------------------------------------------------------------
El juego cuenta con 3 structs para representar los objetos del juego: ship, alien y bullet

Ship
Propiedades
x: Coordenada en el eje x de la nave
y: Coordenada en el eje y de la nave
height: Altura de la nave
width: Ancho de la nave
sprite: Dibujito de la nave

Metodos
DrawShip: Dibuja la nave en la interfaz
EraseShip: Borra la nave de la interfaz
MoveShip: Simula el movimiento de la nave, borrandola de su posicion actual, actualizando sus coordenadas y dibujandola nuevamente
Shoot: Genera un disparo justo encima de la nave

Alien
Propiedades
x: Coordenada en el eje x del alien
y: Coordenada en el eje y del alien
height: Altura del alien
width: Ancho del alien
sprite: Dibujito del alien
life: vida del alien(cantidad de disparos que aguanta)

Metodos
CreateAlien: Inicializa los valores de un alien
DrawAlien: Dibuja el alien en la interfaz
EraseAlien: Borra el alien de la interfaz
MoveAlien: Simula el movimiento del alien
GetHit: Cuando un disparo alcanza a un alien, reduce en 1 su vida, si llega a 0 se elimina el alien y actualiza la puntuacion

Bullet
Propiedades 
x :  Coordenada en el eje x del disparo
y :  Coordenada en el eje y del disparo
active: booleano que representa si el disparo esta activo o no

Metodos
DrawBullet : Dibuja el disparo en la interfaz
EraseBullet : Elimina el disparo de la interfaz
MoveBullet : Simula el movimiento del disparo
DestroyBullet: desactiva y borra de la interfaz el disparo
UpdateBullets: se encarga de mover hacia arriba todos los disparos activos
CheckCollision(alien): chequea si el disparo y alien chocan

-----------------------------------------------------------------------------------------------------
Durante el juego se iran generando filas de aliens que iran bajando hacia el jugador. Para tenerlas almacenadas se usa una estructura de datos de tipo Linked List denominada Horde

Horde
Lista doblemente enlazada donde cada nodo representa un AlienSquad(no es mas que una fila de aliens)

Propiedades
first
last
length

Metodos
NewAlienHorde: Inicializa una Horde con first y last en NULL
Insert: Inserta AL PRINCIPIO de la linked list
Erase: Elimina un nodo pasado por parametro de la linked list

SquadNode
Nodo de la linked list Horde

Propiedades
squad: conjunto de Aliens
prev: nodo anterior
next: nodo siguiente

AlienSquad
Propiedades
aliens: array de aliens
num_aliens: cantidad de aliens del array
height: altura de la squad en la interfaz(posicion en el eje y)

Metodos
CreteAlienSquad: inicializa un ALienSquad 
AllDead: chequea si todos los aliens del squad estan muertos
Landed: chequea si la squad llego a la Tierra(ultima fila en el eje y)

Se tiene un array de bullets de length 10 que almacena los disparos del jugador(solo puedo haber a lo maximo 10 disparos activos al mismo tiempo)

------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Eventos e Hilos

Recursos Compartidos
Durante la ejecucion del juego, hay ciertos recursos que seran utilizados por varios hilos por ello creamos Mutexes para controlar su acceso, estos son:
. La pantalla
. Los datos de la nave
. Los datos de la horda
. Los datos de los disparos

Hilos
Para la ejecucion del juego se utilzan distintos hilos para ejecutar diversos eventos en paralelo, estos son:
.ReactToInput: este hilo se encarga de recibir la entrada del usuario y realizar la accion correspondiente: Mover la nave(WASD), Disparar(ESPACIO), cerrar el juego(ESC)
.MoveBullets: se encarga de actualizar la posicion de los disparos cada cierto tiempo(simulando su movimiento)
.AlienGenerator: si la primera fila esta vacia, genera una cantidad aleatoria de aliens(entre 1 y 10) en posiciones aleatorias de dicha fila y agrega esa squad a la horda( como maximo
la horda puede tener 5 squads pa que no se parta el CPU)
.MoveAliens: se encarga de mover actualizar cada cierto tiempo las posiciones de los aliens, comenzando siempre por la squad mas inferior(la de mas abajo)
CheckColisions: se encarga de chequear si se producen colisiones entre aliens y disparos, en caso afirmativo reduce la vida del alien hasta que muere, si mueren todos los aliens de una squad esta se elimina de la horda
.CheckLanding: chequea si alguna squad de aliens logra llegar a la Tierra, en cuyo caso termina la partida con derrota
.CheckShipHit: chequea si un alien choca contra la nave, en cuyo caso termina la partidad con derrota(aun no funciona)

Para saber cuando termina el juego existe una variable global GameOver la cual se inicia en FALSE y forma la condicion de un bucle que mantiene el programa corriendo mientras GameOver no sea TRUE 

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TALLAS POR HACER
.Pantalla de Inicio(opcional)
.Pantalla de Final de Partida(opcional)
.Ponerle musiquita(opcional)
.Investigar como hacer para que se pueda abrir desde la terminal del sistema(OBLIGATORIO)
.Hacer que los aliens se puedan mover horizontalmente(opcional)
.Hacer que a medida que avance el juego se vuelva mas dificil(aumentar velocidad y vida de los aliens)(opcional)
.Guardar en un archivo las mejores puntuaciones y mostrarlas(opcional)
