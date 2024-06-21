#include<ncurses.h>
#include "math.h"

#define true 1

typedef struct 
{
    int x;
    int y;
    int height;
    char* sprite[4];
} Ship;

void MoveShip(Ship ship, Ship *p , int mov_x , int mov_y);
void DrawShip(Ship ship , Ship *p);
void DeleteShip(Ship ship , Ship *p);
void Shoot(Ship *p);

void ReactToInput(Ship ship, Ship *p)
{
    while (true)
    {
        char ch = (char)getch();
        int mov_x = 0 ;
        int mov_y = 0 ;

        if(ch == ' ')
        {
            // Disparar(ship)
            continue;
        }
        else
        {
            // esta feo pero eficiente
            if(ch == 'w'){mov_y = -1;}
            else if(ch == 's'){mov_y = 1;}
            else if(ch == 'a'){mov_x = -1;}
            else if(ch == 'd'){mov_x = 1;}
            else if((int)ch == 27){break;}
            else{continue;}

            MoveShip(ship, p , mov_x, mov_y);
        }    
    }
}

void MoveShip(Ship ship , Ship *p , int mov_x , int mov_y)
{    
    DeleteShip(ship , p);

    p->x += mov_x;
    p->y += mov_y;

    // para que no se salga de la interfaz
    p->x = max(min(p->x , COLS - 18 ) , 2);
    p->y = max(min(p->y , LINES - 7 ) , 2);
    DrawShip(ship, p);
}

void DrawShip(Ship ship, Ship *p)
{
    ship.x = p->x;
    ship.y = p->y;

    // colores de la nave
    start_color(); 
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    // imprimiendo el dibujo de la nave
    int y = ship.y ;

    for (int i = 0; i < ship.height; i++)
    {
        char* line = ship.sprite[i];
        mvprintw(y , ship.x ,line);
        y++ ;
    }
    
    attroff(COLOR_PAIR(1)); // Desactiva el par de color 1.
    refresh(); // Actualiza la pantalla.

}

void DeleteShip(Ship ship, Ship *p)
{
    ship.x = p->x;
    ship.y = p->y;
    // Borra la nave anteriormente dibujada en la pantalla.
    int y = ship.y ;
    for (int i = 0; i < ship.height; i++)
    {
        mvprintw(y, ship.x , "                 ");
        y++ ;
    }
}
void Shoot(Ship *p)
{
    struct NodoDisparo* nuevoDisparo = malloc(sizeof(struct NodoDisparo));
    if (nuevoDisparo != NULL) {
        nuevoDisparo->x = p->x;
        nuevoDisparo->y = p->y;
        nuevoDisparo->siguiente = listaDisparos;
        listaDisparos = nuevoDisparo;
    }
}




    