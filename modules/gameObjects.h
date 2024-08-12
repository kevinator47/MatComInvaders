#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

#define MAX_BULLETS 10
#define SHIP_HEIGHT 4
#define SHIP_WIDTH 10
#define ALIEN_HEIGHT 2
#define ALIEN_WIDTH 5

#define ESC 27
#define TRUE 1
#define FALSE 0

// --------------------  STRUCTS  ----------------------------------
struct ship {
    int x;
    int y;
    int height;
    int width;
    char* sprite[SHIP_HEIGHT];
};
struct bullet {
    int x;
    int y;
    int active;
};
struct alien {
    int x;
    int y;
    int height;
    int width;
    char* sprite[ALIEN_HEIGHT];
    int life;
};
// --------------------  PROTOTYPES  ----------------------------------
void DrawShip(struct ship* ship);
void EraseShip(struct ship* ship);
void MoveShip(struct ship* ship, int movX, int movY);

void DrawBullet(struct bullet* b);
void EraseBullet(struct bullet* b);
void MoveBullet(struct bullet* b);

void DrawAlien(struct alien* alien);
void EraseAlien(struct alien* alien);
int MoveAlien(struct alien* alien, int movX);
void CreateAlien(struct alien* alien, int num_aliens, int i, int y);
int GetRandomCoo(int i, int num_aliens) ;

void Shoot(struct ship* ship);
int CenterPosition(struct ship* ship);
void UpdateBullets();

int FirstFit();
// --------------------  GLOBALS  ----------------------------------

int firstAvailablePosition = 0;
struct bullet bullets[MAX_BULLETS];
int score = 0 ;
// --------------------  SHIP  ----------------------------------
void MoveShip(struct ship* ship, int movX, int movY) 
{
    EraseShip(ship);   

    ship->x = min(max(ship->x + movX, 1), COLS - ship->width - 1);
    ship->y = min(max(ship->y + movY, 1), LINES - ship->height - 1);

    DrawShip(ship);     
}
void EraseShip(struct ship* ship) 
{
    char spaces[ship->width + 1];
    GenerateSpaceBlock(spaces, ship->width);

    for (int i = 0; i < ship->height; i++) 
    {
        mvprintw(ship->y + i, ship->x, "%s", spaces);
    }
    refresh();
}
void DrawShip(struct ship* ship) 
{
    attron(COLOR_PAIR(1));
    for (int i = 0; i < ship->height; i++) 
    {
        mvprintw(ship->y + i, ship->x, "%s", ship->sprite[i]);
    }
    attroff(COLOR_PAIR(1));
    refresh();
}
void Shoot(struct ship* ship) 
{
    int bulletIndex = FirstFit();
    if (bulletIndex != -1) {
        bullets[bulletIndex].active = 1;
        bullets[bulletIndex].x = CenterPosition(ship);
        bullets[bulletIndex].y = ship->y - 1;
        DrawBullet(&bullets[bulletIndex]);
    }
}
int FirstFit() 
{
    for (int i = 0; i < MAX_BULLETS; i++) 
    {
        if (!bullets[i].active) 
            return i;
    }
    return -1;
}
int CenterPosition(struct ship* ship) 
{
    return ship->x + (ship->width / 2) - 1 ;
}

// --------------------  BULLET  ----------------------------------
void MoveBullet(struct bullet* b) 
{
    EraseBullet(b);
    b->y--;
    if (b->y < 1)       // Si el disparo alcanza el tope de la pantalla se desactiva
        b->active = 0; 
    else 
        DrawBullet(b);
}
void DrawBullet(struct bullet* b) 
{
    attron(COLOR_PAIR(3));
    mvprintw(b->y, b->x, "*");
    attroff(COLOR_PAIR(3));
    refresh();
}

void EraseBullet(struct bullet* b) 
{
    mvprintw(b->y, b->x, " ");
    refresh();
}
void DestroyBullet(struct bullet* b)
{
    b->active = 0;
    EraseBullet(b);
}
void UpdateBullets() 
{
    for (int i = 0; i < MAX_BULLETS; i++) 
    {
        if (bullets[i].active) 
            MoveBullet(&bullets[i]);
    }
}
// --------------------  ALIEN  ----------------------------------
void CreateAlien(struct alien* alien, int num_aliens, int i, int y) 
{
    alien->x = GetRandomCoo(i, num_aliens);   // Inicializar la ubicación x del alien
    alien->y = y;                             // Inicializar la posición y del alien
    alien->height = ALIEN_HEIGHT;             // Inicializar la altura del alien
    alien->width = ALIEN_WIDTH;               // Inicializar el ancho del alien
    alien->sprite[0] = " \\ / ";              // Inicializar el sprite del alien
    alien->sprite[1] = "(x_x)";               // Inicializar el sprite del alien
    alien->life = 3;                          // Inicializar la vida del alien
    firstAvailablePosition = alien->x + ALIEN_WIDTH;
    DrawAlien(alien);
}

int MoveAlien(struct alien* alien , int movX) 
{
    EraseAlien(alien);

    alien->x = min(max(alien->x + movX, 1), COLS - alien->width - 1);
    alien->y++ ;
    
    if(alien->y <= (LINES - ALIEN_HEIGHT - 1) && alien->life > 0) {
        DrawAlien(alien);
    }
}

void DrawAlien(struct alien* alien) 
{
    attron(COLOR_PAIR(2));
    for (int i = 0; i < alien->height; i++) 
    {
        mvprintw(alien->y + i, alien->x, "%s", alien->sprite[i]);
    }
    attroff(COLOR_PAIR(2));
    refresh();
}

void EraseAlien(struct alien* alien) 
{
    char spaces[alien->width + 1];
    GenerateSpaceBlock(spaces, alien->width);
    for (int i = 0; i < alien->height; i++) 
    {
        mvprintw(alien->y + i, alien->x, "%s", spaces);
    }
    refresh();
}
void GetHit(struct alien* alien)
{
    alien->life--;
    if(alien->life <= 0)
    {
        EraseAlien(alien);
        score++;
    }
}

void CheckCollision(struct alien* alien, struct bullet* bullet)
{
    if(alien->x <= bullet->x && bullet->x <= alien->x + alien->width && alien->y <= bullet->y && bullet->y <= alien->y + alien->height)
    {
        DestroyBullet(bullet);
        GetHit(alien);
    }
}
int CheckShipCollision(struct ship* ship, struct alien* alien)
{
    // Vertical collison
    int collision = CheckIntersection(ship->y, ship->y + ship->height, alien->y, alien->y + alien->height);

    // Horizontal collison
    collision = collision && CheckIntersection(ship->x, ship->x + ship->width, alien->x, alien->x + alien->width);

    if (collision)
    {
        GetHit(alien);
        EraseShip(ship);
        return 1;
    }
    return 0;
}
int GetRandomCoo(int i, int num_aliens) {
    int max_x = COLS - ((num_aliens - i + 1) * ALIEN_WIDTH);
    unsigned int seed = time(NULL);
    return rand_r(&seed) % (max_x - firstAvailablePosition + 1) + firstAvailablePosition;
}

// --------------------  HORDE  ----------------------------------

// Structs
typedef struct {
    struct alien* aliens;   // array de aliens
    int height;             // altura de la línea en la pantalla
    int num_aliens;         // tamaño del array de aliens
} AlienSquad;

typedef struct SquadNode {
    AlienSquad* squad;
    struct SquadNode* prev;
    struct SquadNode* next;
} SquadNode;

typedef struct {
    SquadNode* first;       // referencia al primer squad de la horda
    SquadNode* last;        // referencia al último squad de la horda
    int length;             // cantidad de elementos
} AlienHorde;

// Methods
AlienHorde* NewAlienHorde() 
{
    AlienHorde* horde = (AlienHorde*) malloc(sizeof(AlienHorde));
    horde->first = NULL;
    horde->last = NULL;
    return horde;
}
void Insert(AlienHorde* horda, AlienSquad* squad) 
{
    SquadNode* nuevo_nodo = (SquadNode*) malloc(sizeof(SquadNode));
    nuevo_nodo->squad = squad;
    
    if (horda->last == NULL) {          // Si horda esta vacia
        horda->first = nuevo_nodo;       
        horda->last = nuevo_nodo;
        nuevo_nodo->prev = NULL;
        nuevo_nodo->next = NULL;
    } else {
        nuevo_nodo->next = horda->first;     // Se inserta al inicio
        nuevo_nodo->prev = NULL;
        horda->first->prev = nuevo_nodo;
        horda->first = nuevo_nodo;
    }
    horda->length += 1;
}
void Erase(AlienHorde* horda, SquadNode* nodo) 
{
    if (nodo->prev == NULL)         // El nodo a eliminar es el primero de la lista 
    {           
        horda->first = nodo->next;
        if (horda->first != NULL)
            horda->first->prev = NULL;
    } 
    else if (nodo->next == NULL)     // El nodo a eliminar es el último de la lista 
    {
        horda->last = nodo->prev;
        if (horda->last != NULL) 
            horda->last->next = NULL;
    } 
    else                              // El nodo a eliminar está en medio de la lista
    {
        nodo->prev->next = nodo->next;
        nodo->next->prev = nodo->prev;
    }
    free(nodo);
    horda->length -= 1;
}
AlienSquad* CreateAlienSquad(int num_aliens) 
{
    AlienSquad* squad = (AlienSquad*) malloc(sizeof(AlienSquad));
    squad->aliens = (struct alien*) malloc(num_aliens * sizeof(struct alien));
    squad->num_aliens = num_aliens;
    squad->height = 2;
    for (int i = 0; i < num_aliens; i++)
    {
        CreateAlien(&squad->aliens[i], num_aliens, i, squad->height);
    }
    firstAvailablePosition = 0;

    return squad;
}
int AllDead(SquadNode* squad)
{
    for(int i = 0; i < squad->squad->num_aliens; i++)
    {
        if(squad->squad->aliens[i].life > 0)
            return FALSE;
    }
    return TRUE;
}
int Landed(SquadNode* squad)
{
    return (squad->squad->aliens[0].y == (LINES - ALIEN_HEIGHT - 1));
}
// ------------------------------------------------------------------------------------