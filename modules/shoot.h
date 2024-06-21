#include <ncurses.h>

typedef struct 
{
    int x;
    int y;
} Shoot;

void MoveShoot(Shoot *p)
{
    DeleteShoot(p);
    p->y-- ;
    DrawShoot(p);
}

void DrawShoot(Shoot *p)
{
    mvprintw(p->x,p->y,"*");
}

void DeleteShoot(Shoot *p)
{
    mvprintw(p->x,p->y,"");
}