#include <ncurses.h>
#include <ctype.h>
#include "modules/headers/ship.h"
#include "modules/ship.c"

#define TRUE 1

void InitializateSettings();

int main() {
    InitializateSettings();

    // Inicializar la nave del jugador
    int x = COLS / 2 -10;
    int y = LINES - 5;

    struct ship playerShip = {x,y, 4, {"   (^)   " , "( o o o )", " ------- ", " /  |  \\ "} };

    // Dibujar la nave del jugador
    DrawShip(&playerShip);
    refresh();

    while (TRUE)
    {
        if(ReactToInput(&playerShip))
        {
            break;
        }
    }
    
    // Cerrar ncurses y finalizar el juego
    endwin();
    return 0;
}

void InitializateSettings()
{
    initscr(); // Inicia la ventana de la pantalla. 
    start_color(); // Inicia el modo de color.
    curs_set(0); // Oculta el cursor.
    raw(); // Desactiva el buffering de línea.
    keypad(stdscr, TRUE); // Habilita el uso de teclas especiales como las flechas.
    noecho(); // No muestra los caracteres de entrada en la pantalla.
    box(stdscr, 0, 0); // Dibuja un recuadro alrededor de la ventana estándar.
}

