#include <ncurses.h>
#include "headers/bullet.h"
#define MOVEMENT_DELAY 100

void DrawBullet(struct bullet* b) {
    mvaddch(b->y, b->x, '*');
}

void EraseBullet(struct bullet* b) {
    mvaddch(b->y, b->x, ' ');
}

void MoveBullet(struct bullet* b) {
    EraseBullet(b);
    if(b->y > 1)
    {
        b->y -- ;
        DrawBullet(b);
    }
        
}

void UpdateBullets(struct bullet* bullets, int numBullets) {
    for (int i = 0; i < numBullets; i++) {
        
        MoveBullet(&bullets[i]);
    }
}
int CheckOutOfRange(struct bullet* b) {
    if (b->y <= 0) {
        return 1;
    }
    return 0;
}

