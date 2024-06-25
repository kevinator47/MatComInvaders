#ifndef BULLET_H
#define BULLET_H

#define MAX_BULLETS 10

struct bullet {
    int x;
    int y;
    int active;
};
void DrawBullet(struct bullet* b);
void EraseBullet(struct bullet* b);
void MoveBullet(struct bullet* b);
void UpdateBullets(struct bullet* bullets);

#endif