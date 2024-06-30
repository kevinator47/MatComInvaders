#include <ncurses.h>
#include "headers/ship.h"
#include "headers/bullet.h"
#include "headers/math.h"
#include "headers/threadProps.h"
#define ESC 27

// PROTOTYPES
void GenerateSpaceBlock(char* block, int width);
int FirstFit(struct bullet* bullets);

//-----------------------------------------------------------------------------------------
void DrawShip(struct ship* ship) {
    /*
    Se encarga de dibujar la nave en la consola
      .Setea los colores con los que se va a dibujar
      .Itera sobre el sprite de la nave y lo va printeando en las coordenadas de la nave
      .Desactiva los colores y refresca la pantalla
    */

    // Setea los colores
    start_color(); 
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    
    // Dibuja la nave
    for (int i = 0; i < ship->height; i++) {
        mvprintw(ship->y + i, ship->x, "%s",ship->sprite[i]);
    }
    attroff(COLOR_PAIR(1));               // Desactiva el par de color 1.
    refresh();                            // Actualiza la pantalla.
}

void EraseShip(struct ship* ship) {
    /*
    Borra la nave de la consola
     .Por cada linea de altura de la nave imprime un espacio en blanco del mismo ancho que la nave  
    */

    // Genera un bloque de espacios en blanco para cubrir la nave
    char spaces[ship->width + 1];
    GenerateSpaceBlock(spaces, ship->width);

    // Borra la nave
    for (int i = 0; i < ship->height; i++) {
        mvprintw(ship->y + i, ship->x, "%s",spaces);
    }
}

void MoveShip(struct ship* ship, int movX, int movY) {
    /*
    Mueve la nave en dependencia de la direccion que recibe por parametros
    @params :
      ship : puntero hacia la nave
      movX : movimiento de la nave en el eje X 
      movY : movimiento de la nave en el eje Y 
    
    .Borra la nave que se encuentra actualmente en pantalla.
    .Actualiza su posicion, evitando que se salga de pantalla.
    .La vuelve a dibujar en la nueva posicion. 
    */
    EraseShip(ship);
    
    // Actualiza las coordenadas de la nave
    ship->x = max(min(ship->x + movX , COLS - 18 ) , 1);
    ship->y = max(min(ship->y + movY , LINES - 5 ) , 1);
    
    // Dibuja la nave en la nueva posición
    DrawShip(ship);
}

//-----------------------------------------------------------------------------
void Shoot(struct ship* ship, struct bullet* bullets) {
    /*
    Genera un disparo justo encima de la nave
      .Busca en la lista de disparos el primer espacio disponible
      .Si se encuentra alguno genera un disparo en la posicion de la nave y dibujalo
    */
    int pos = FirstFit(bullets);
    if(pos != -1) {
        bullets[pos].active = 1;
        bullets[pos].x = ship->x + SHIP_WIDTH / 2;
        bullets[pos].y = ship->y - 1;
        DrawBullet(&bullets[pos]);
    }
}
//-----------------------------------------------------------------------------
void* ReactToInput(void* params) {
    /*
    Recibe la entrada del usuario y hace que la nave reaccione a ella
    @params :
      ship : puntero hacia la nave
      bullets : puntero hacia la lista de disparos
      abort : flag para detener el juego
    @actions :
      ' ' : Dispara
      'w', 'a', 's', 'd' : Movimiento
      ESC : Termina el juego
    */
    
    // Desestructurando el parametro
    struct InputThreadProps* p = (struct InputThreadProps*) params;
    struct ship* ship = p->ship;
    struct bullet* bullets = p->bullets;
    int* endgame = p->endgame;

    int mov_x, mov_y;
    char c ;
    
    while (!endgame)
    {
        c = tolower(getch());
        switch (c) {
            case ' ':
                Shoot(ship, bullets);
                break;
            case 'w':
            case 'a':
            case 's':
            case 'd':
                GetMovement(c, &mov_x, &mov_y);
                MoveShip(ship, mov_x, mov_y);
                break;
            case ESC:
                // region critica creo
                *endgame = 1;
                break;
            default:
                break;
        }
    }
    return NULL;
}
//-----------------------------------------------------------------------------
void GenerateSpaceBlock(char* block, int width) {
    /* Genera un bloque de espacios en blanco de un ancho especifico */
    for (int i = 0; i < width; i++) {
        block[i] = ' ';
    }
    block[width] = '\0';
}

int FirstFit(struct bullet* bullets) {
    /* Busca en la lista de disparos el primer espacio disponible */
    for (int i = 0; i < MAX_BULLETS; i++) {
        if(!bullets[i].active) {
            return i;
        }
    }
    return -1;
}