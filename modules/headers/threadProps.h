#ifndef THREAD_PROPS_H
#define THREAD_PROPS_H
struct InputThreadProps {
    struct ship* ship;
    struct bullet* bullets;
    int* endgame;
};
struct BulletThreadProps {
    struct bullet* bullets;
    int endgame;
};
#endif