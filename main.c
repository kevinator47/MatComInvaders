#include <ncurses.h>
#include <pthread.h>
#include <unistd.h> 

#include <ctype.h>
#include "./modules/gameObjects.h"

#define BULLET_SPEED 30000
#define ALIEN_SPEED  500000

typedef struct {
    AlienHorde* horde;
    struct ship* playerShip;
} CheckHitParams;

// Prototypes

struct ship Start();
void BuildUI();
struct ship BuildShip();

void* moveBullets(void* arg);
void* ReactToInput(void* arg);
void* GenerateNewAlienSquad(void* arg);
void* MoveAliens(void* arg);
void* CheckCollisions(void* arg);
void* CheckLanding(void* arg);
void* CheckShipHit(void* arg);

void  MoveHorde(AlienHorde* horde);
AlienSquad* CreateAlienSquad(int num_aliens);

// Globals
int gameOver = FALSE;

// Mutexes
pthread_mutex_t screenMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t shipDataMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bulletDataMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t alienDataMutex = PTHREAD_MUTEX_INITIALIZER;

int main() 
{
    struct ship playerShip = Start();
    AlienHorde* horde = NewAlienHorde();
    DrawShip(&playerShip);

    CheckHitParams params;
    params.horde = horde;
    params.playerShip = &playerShip;
    
    // Crea los hilos
    pthread_t inputThread,bulletsThread, alienGeneratorThread, moveAliensThread, checkCollisionsThread;
    pthread_t shipCollisionThread, alienLandingThread;
    
    pthread_create(&inputThread, NULL, ReactToInput, &playerShip);
    pthread_create(&bulletsThread, NULL, moveBullets, NULL);
    pthread_create(&alienGeneratorThread, NULL, GenerateNewAlienSquad, horde);
    pthread_create(&moveAliensThread, NULL, MoveAliens, horde);
    pthread_create(&checkCollisionsThread, NULL, CheckCollisions, horde);
    pthread_create(&alienLandingThread, NULL, CheckLanding, horde);
    pthread_create(&shipCollisionThread, NULL, CheckShipHit, &params);

    while (!gameOver)
    {
        continue;
    }
    endwin();
    if (horde->last==NULL)
    {
        printf("Null\n");
    }
    printf("Has eliminado %d aliens.\n", score);
    
    return 0;
}
//---------------------------------------------------------------------
struct ship Start() {
    BuildUI();
    return BuildShip();
}

void BuildUI() {
    // Crea la ventana del juego e inicializa los ajustes 
    initscr();                                  // Inicia la ventana de la pantalla. 
    start_color();                              // Inicia el modo de color.
    init_pair(1, COLOR_RED, COLOR_BLACK);       // Definir el identificador del color rojo
    init_pair(2, COLOR_GREEN, COLOR_BLACK);     // Definir el identificador del color verde
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);    // Definir el identificador del color azul
    curs_set(0);                                // Oculta el cursor.
    raw();                                      // Desactiva el buffering de línea.
    keypad(stdscr, TRUE);                       // Habilita el uso de teclas especiales como las flechas.
    noecho();                                   // No muestra los caracteres de entrada en la pantalla.
    srand(time(NULL));                          // Inicializa el generador de números pseudoaleatorios.
    box(stdscr, 0, 0);                          // Dibuja un recuadro alrededor de la ventana estándar.
    refresh();                                  // Refresca la pantalla.
}

struct ship BuildShip() 
{
    // Crea la nave del jugador
    int x = COLS / 2 - SHIP_WIDTH;
    int y = LINES - SHIP_HEIGHT - 1;
    struct ship theShip = {x, y, 4, 10, {
        "   (^)   ", 
        "( o o o )", 
        " ------- ", 
        " /  |  \\ "
    }};

    return theShip;
}

void* ReactToInput(void* arg)
{
    struct ship* ship = (struct ship*)arg;
    while(TRUE) 
    {
        int key = tolower(getch()); // Obtiene la tecla presionada por el usuario
        
        pthread_mutex_lock(&shipDataMutex);
        pthread_mutex_lock(&screenMutex);
        
        // Actualiza la posición de la nave según la tecla presionada
        switch (key) {
            case 'w':
                // Mueve la nave hacia arriba
                MoveShip(ship, 0, -1);
                break;
            case 'a':
                // Mueve la nave hacia la izquierda
                MoveShip(ship, -1, 0);
                break;
            case 's':
                // Mueve la nave hacia abajo
                MoveShip(ship, 0, 1);
                break;
            case 'd':
                // Mueve la nave hacia la derecha
                MoveShip(ship, 1, 0);
                break;
            case ' ' :
                Shoot(ship);
                break;
            case ESC:
                // Sale del juego
                gameOver = TRUE;
                endwin();
                exit(0);
                break;
            default:
                // No se ha presionado ninguna tecla util
                break;
        }

        pthread_mutex_unlock(&shipDataMutex);
        pthread_mutex_unlock(&screenMutex);    
    }
}

void* moveBullets(void* arg) 
{
    while (TRUE)
    {
        pthread_mutex_lock(&bulletDataMutex);
        pthread_mutex_lock(&screenMutex);
        
        UpdateBullets();
        
        pthread_mutex_unlock(&screenMutex);
        pthread_mutex_unlock(&bulletDataMutex);
        
        usleep(BULLET_SPEED);
    }    
}
void* GenerateNewAlienSquad(void* arg) 
{
    AlienHorde* horde = (AlienHorde*) arg;
    while (TRUE) 
    {
        pthread_mutex_lock(&alienDataMutex);
        pthread_mutex_lock(&screenMutex);

        if ((horde->first == NULL || horde->first->squad->height > 4) && horde->length < 6) 
        {
            unsigned int seed = time(NULL);
            int num_aliens = rand_r(&seed) % 10 + 1;       
            AlienSquad* new_squad = CreateAlienSquad(num_aliens);
            Insert(horde, new_squad);   // insertar el nuevo squad como primer elemento
        }

        pthread_mutex_unlock(&screenMutex);
        pthread_mutex_unlock(&alienDataMutex);
        
        usleep(1000000);    // Dormir un poco para no consumir demasiado CPU
    }
    return NULL;
}
void* MoveAliens(void* arg) 
{
    // Mueve cada squad de aliens de abajo hacia arriba
    AlienHorde* horde = (AlienHorde*) arg;
    while (TRUE)
    {
        pthread_mutex_lock(&alienDataMutex);
        pthread_mutex_lock(&screenMutex);

        MoveHorde(horde);
        
        pthread_mutex_unlock(&screenMutex);
        pthread_mutex_unlock(&alienDataMutex);
        
        usleep(ALIEN_SPEED);
    }
    return NULL;
}
void MoveHorde(AlienHorde* horde) 
{
    SquadNode* current_squad = horde->last;
    while (current_squad != NULL) {
        for (int i = 0; i < current_squad->squad->num_aliens; i++) 
        {
            MoveAlien(&current_squad->squad->aliens[i],0);
        }
        current_squad->squad->height ++;
        current_squad = current_squad->prev;
    }
    refresh();
}

void* CheckCollisions(void* arg) 
{
    while (TRUE)
    {
        pthread_mutex_lock(&alienDataMutex);
        pthread_mutex_lock(&bulletDataMutex);

        AlienHorde* horde = (AlienHorde*) arg;
        SquadNode* current_squad = horde->last;
        while (current_squad != NULL)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if(bullets[i].active)
                {
                    int h = current_squad->squad->height;
                    if(h <= bullets[i].y && bullets[i].y <= h + ALIEN_HEIGHT)
                    {
                        for(int j = 0; j < current_squad->squad->num_aliens; j++)
                        {
                            if(current_squad->squad->aliens[j].life > 0)
                            {
                                pthread_mutex_lock(&screenMutex);
                                CheckCollision(&current_squad->squad->aliens[j], &bullets[i]);
                                refresh();
                                pthread_mutex_unlock(&screenMutex);
                            }                       
                        }
                    }
                }  
            }
            current_squad = current_squad->prev;
            SquadNode* lastChecked;

            if(current_squad != NULL)
            {
                lastChecked = current_squad->next; 
            }
            else
            {
                lastChecked = horde->first;
            }

            if(AllDead(lastChecked))
            {
                Erase(horde,lastChecked);
            }
            
        }
        pthread_mutex_unlock(&alienDataMutex);
        pthread_mutex_unlock(&bulletDataMutex);
    }
    return NULL;
}
void* CheckLanding(void* arg)
{
    while (TRUE)
    {
        pthread_mutex_lock(&alienDataMutex);
        AlienHorde* horde = (AlienHorde*) arg;
        if(horde->last != NULL && Landed(horde->last))
        {
            gameOver = TRUE;
        }
        pthread_mutex_unlock(&alienDataMutex);
    }
    return NULL;
}

void* CheckShipHit(void* arg)
{
    CheckHitParams* params = (CheckHitParams*) arg;
    AlienHorde* horde = params->horde;
    struct ship* playerShip = params->playerShip;
    while (TRUE)
    {
        pthread_mutex_lock(&alienDataMutex);
        pthread_mutex_lock(&shipDataMutex);
        pthread_mutex_lock(&screenMutex);
        SquadNode* n = horde->last;

        while (n != NULL)
        {
            for (int i = 0; i < n->squad->num_aliens; i++)
            {
                if (n->squad->aliens[i].life > 0 && CheckShipCollision(playerShip,&n->squad->aliens[i]))
                {
                    gameOver = true ;
                    pthread_mutex_unlock(&alienDataMutex);
                    pthread_mutex_unlock(&shipDataMutex);
                    pthread_mutex_unlock(&screenMutex);
                    return NULL;
                }
            }
            n = n->prev;
        }
        pthread_mutex_unlock(&alienDataMutex);
        pthread_mutex_unlock(&shipDataMutex);
        pthread_mutex_unlock(&screenMutex);
        return NULL;
        
    }
    

}