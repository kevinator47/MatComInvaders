#include "headers/ship.h"
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>

void DrawAliens(struct ship* aliens, int number);
void CheckCollision(struct ship* aliens, struct bullet b);

void DrawAliens(struct ship* aliens, int number)
{
    int x = 10;
    int y = 10;
    for (int i = 0; i < number; i++)
    {
        struct ship alien = {x,y,2,5,{" # # ","|x_x|"}};
        aliens[i] = alien; 
        DrawShip(&alien);
        x += 7;
    }
}

void CheckCollision(struct ship* aliens, struct bullet b)
{
    //Cambiar el 10 por el tamanno de aliens
    for (int i = 0; i < 6; i++)
    {
        if (aliens[i].y == b.y && b.x >= aliens->x && b.x <= aliens->x + aliens->width)
        {
            struct ship alien = aliens[i];
            EraseShip(&alien);
        }
    }
}