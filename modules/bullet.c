#include <ncurses.h>
#include "headers/bullet.h"
#define MOVEMENT_DELAY 100

void DrawBullet(struct bullet* b) {
    /*
    Pinta el disparo en pantalla printeando un asterisco en sus coordenadas
    @param b : pointer hacia el disparo
    */
    mvaddch(b->y, b->x, '*');
}

void EraseBullet(struct bullet* b) {
    /*
    Borra el disparo de la pantalla printeando un espacio en blanco en su lugar 
    @param b : pointer hacia el disparo
    */
    mvaddch(b->y, b->x, ' ');
}

void MoveBullet(struct bullet* b) {
    /*
    Simula el movimiento del disparo :
      .Primeramente, borra el disparo de la pantalla
      .Luego, chequea si el disparo se mantendra dentro de la pantalla al moverse
        .Si se mantiene: actuializa su coordenada en el "eje y" y lo vuelve a pintar
        .Si se sale de la pantalla : desactiva el disparo
    */
    EraseBullet(b); 
    
    if(b->y > 1) {                 // Si el diparo aun esta dentro de la pantalla
        b->y -- ;                  // se mueve hacia arriba
        DrawBullet(b);             // vuelve a pintar el disparo
    }
    else {                         // Si se sale de la pantalla
        b->active = 0;             // Desactiva el disparo
    }        
}

void UpdateBullets(struct bullet* bullets) {
    /* 
     Se encarga de actualizar los movimientos de los disparos
       . Itera sobre la lista de disparos moviendo los que se encuentren activos 
    */
    for (int i = 0; i < MAX_BULLETS; i++) {
        if(bullets[i].active) {
            MoveBullet(&bullets[i]);
        }
    }
}


