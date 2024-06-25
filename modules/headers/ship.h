#ifndef SHIP_H
#define SHIP_H

#define SHIP_HEIGHT 4
#define SHIP_WIDTH 10

struct ship {
    int x;
    int y;
    int height;
    int width;
    char* sprite[SHIP_HEIGHT];
};

void DrawShip(struct ship* ship);
void EraseShip(struct ship* ship);
void MoveShip(struct ship* ship, int movX, int movY);
#endif