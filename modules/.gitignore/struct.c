#include <stdio.h>
typedef struct 
{
    int x;
    int y;
    int height;
    const char* sprite[4];
}Ship;


int main()
{
    Ship ship = (Ship){0,0,4,{"    (^)    " ," ( o o o ) ","  ------- ","  /  |  \\  ",}};

    for (int i = 0; i < 4; i++)
    {
        printf("%s\n", ship.sprite[i]);
    }
    
    return 0;
}
