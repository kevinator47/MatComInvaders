#include <pthread.h>
#include <ncurses.h>
#include <ctype.h>
#include "modules/ship.c"
#include "modules/bullet.c"
#include "modules/headers/threadProps.h"

#define TRUE 1
#define FALSE 0

void InitializateSettings();
void InitializateBullets(struct bullet* bullets);



int main() {
    struct bullet bullets[MAX_BULLETS];  
    InitializateSettings();
    InitializateBullets(bullets);
    int endgame = FALSE;

    // Inicializar y dibujar la nave del jugador
    int x = COLS / 2 - SHIP_WIDTH;
    int y = LINES - SHIP_HEIGHT - 1;
    struct ship playerShip = {x, y, 4, 10, {"   (^)   " , "( o o o )", " ------- ", " /  |  \\ "} };
    DrawShip(&playerShip);
    refresh();
    
    pthread_t ship_thread, bullets_thread;
    
    struct InputThreadProps ship_params = {&playerShip, bullets, &endgame};
    struct BulletThreadProps bullets_params = {bullets, endgame};

    pthread_create(&ship_thread, NULL, ReactToInput, (void *)&ship_params);
    pthread_create(&bullets_thread, NULL, UpdateBullets, (void *)&bullets_params);

    pthread_join(ship_thread, NULL);
    pthread_join(bullets_thread, NULL);
    
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
