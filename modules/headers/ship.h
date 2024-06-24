#ifndef SHIP_H
#define SHIP_H

struct ship {
    int x;
    int y;
    int height;
    char* sprite[4];
};

void DrawShip(struct ship* ship);
void EraseShip(struct ship* ship);
void MoveShip(struct ship* ship, int movX, int movY);
#endif