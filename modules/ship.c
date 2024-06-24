#include <ncurses.h>
#include "headers/ship.h"
#include "headers/math.h"

#define ESC 27
void DrawShip(struct ship* ship) {
    
    // Inicia colores para la nave
    start_color(); 
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    
    // Utiliza mvprintw de la librería ncurses para pintar la nave en la posición correspondiente
    for (int i = 0; i < ship->height; i++) {
        mvprintw(ship->y + i, ship->x, "%s",ship->sprite[i]);
    }
    
    attroff(COLOR_PAIR(1)); // Desactiva el par de color 1.
    refresh(); // Actualiza la pantalla.
}

void EraseShip(struct ship* ship) {
    // Utiliza mvprintw de la librería ncurses para borrar la nave en la posición correspondiente
    for (int i = 0; i < ship->height; i++) {
        mvprintw(ship->y + i, ship->x, "                 ");
    }
}

void MoveShip(struct ship* ship, int movX, int movY) {
    // Borra la nave en su posición actual
    EraseShip(ship);
    
    // Actualiza las coordenadas de la nave
    ship->x = max(min(ship->x + movX , COLS - 18 ) , 1);
    ship->y = max(min(ship->y + movY , LINES - 5 ) , 1);
    
    // Dibuja la nave en la nueva posición
    DrawShip(ship);
}

//-------------------------------------------------------------------
int ReactToInput(struct ship* ship) {
    char c = tolower(getch());
    int mov_x, mov_y;
    switch (c)
    {
    case ' ':
        //Shoot()
        break;

    case 'w':
    case 'a':
    case 's':
    case 'd':
        GetMovement(c, &mov_x, &mov_y);
        MoveShip(ship, mov_x, mov_y);
        break;

    case ESC:
        return 1;
    
    default:
        break;
    }
    return 0 ;
}

