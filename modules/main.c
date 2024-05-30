#include <ncurses.h> // Incluye la biblioteca ncurses, que proporciona funciones para manipular la pantalla de texto en un entorno de terminal de forma avanzada.
#include"Ship.h"
#include<stdio.h>

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


int main() {
    
    int x = 10, y = 10; // Coordenadas de la posición inicial de la nave en la pantalla.
    InitializateSettings();

    // Inicializar Nave
    Ship ship=(Ship){x,y, 4, {"    (^)    " , " ( o o o ) ", "  ------- ", "  /  |  \\  "} };

    DrawShip(ship, &ship);
    attroff(COLOR_PAIR(1)); // Desactiva el par de color 1.
    refresh(); // Actualiza la pantalla.
    
    ReactToInput(ship , &ship);
        
    endwin(); // Finaliza la ventana de la pantalla.
    return 0; // Retorna 0 para indicar que el programa se ejecutó correctamente.
}

