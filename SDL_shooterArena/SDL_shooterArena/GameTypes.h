#include "debugmalloc.h"
#ifndef GameTypes
#define GameTypes
typedef struct Projectile {
    int x, y;
    bool hasHit;
    SDL_Texture* Tex;
    int speed;
} Projectile;

typedef struct Enemy {
    int x, y;
    int health;
    Uint32 lastShot;
    int rof; //rate of fire (valojaban x milliszekundum amennyi idonkent lohet az ellenseg)
    Uint32 respawn;
    bool isDead;
}Enemy;

typedef struct Player {
    int x, y;
    int health;
    Uint32 lastShot;
    bool isWalking;
    bool hasShot;
    bool canShoot;
    SDL_Texture* currentTex;
    bool isDead;
    int totalDistance;
    int projectilesShot;
    int projectilesLanded;
    int score;
} Player;

typedef struct Game {
    SDL_Renderer* renderer;
    Player* p;
    SDL_Texture* bgT;
    SDL_Texture* playerProjTex;
    SDL_Texture* enemyTex;
    Projectile* projectiles;
    Enemy* enemies;
    Projectile* enemyProjectiles;
    SDL_Texture* enemyProjTex;
    SDL_Texture* healthTex;
}Game;
#endif 

