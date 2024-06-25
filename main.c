#include <ncurses.h>
#include <ctype.h>
#include "modules/headers/ship.h"
#include "modules/ship.c"
#include "modules/bullet.c"

#define TRUE 1


void InitializateSettings();
void InitializateBullets(struct bullet* bullets);

int main() {

    struct bullet bullets[MAX_BULLETS];  
    InitializateSettings();
    InitializateBullets(bullets);

    // Inicializar y dibujar la nave del jugador
    int x = COLS / 2 - SHIP_WIDTH;
    int y = LINES - SHIP_HEIGHT - 1;
    struct ship playerShip = {x, y, 4, 10, {"   (^)   " , "( o o o )", " ------- ", " /  |  \\ "} };
    DrawShip(&playerShip);
    refresh();

    // Se inicializa el juego(aqui se crearian hilos)
    while (TRUE) {
        if(ReactToInput(&playerShip , bullets)) {
            break;
        }
        UpdateBullets(bullets);
        usleep(MOVEMENT_DELAY);
    }
    
    endwin();   // Cerrar ncurses y finalizar el juego
    return 0;
}

void InitializateSettings() {
    initscr(); // Inicia la ventana de la pantalla. 
    start_color(); // Inicia el modo de color.
    curs_set(0); // Oculta el cursor.
    raw(); // Desactiva el buffering de línea.
    keypad(stdscr, TRUE); // Habilita el uso de teclas especiales como las flechas.
    noecho(); // No muestra los caracteres de entrada en la pantalla.
    box(stdscr, 0, 0); // Dibuja un recuadro alrededor de la ventana estándar.
}

void InitializateBullets(struct bullet* bullets) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
}
