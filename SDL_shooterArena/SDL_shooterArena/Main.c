#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "PlayerUtils.h"
#include "GameFunctions.h"
#include "debugmalloc.h"


int main(int argc, char* argv[]) {
    //Ablak es renderer inicializalasa
    SDL_Window* window;
    SDL_Renderer* renderer;

    FILE* scoreFile;

    //Ablak letrehozasa - init.c-bol inheritalva
    sdl_init("Mage-Shooter-Arena2D!", 1280, 720, &window, &renderer);

    //Hatter
    SDL_Texture* bgTex = initTexture(renderer, "assets/game_background_1.png");
    //PlayerIdle
    SDL_Texture* playerTex = initTexture(renderer, "assets/Idle.png");
    //PlayerWalk
    SDL_Texture* playerWalkTex = initTexture(renderer, "assets/Walk.png");
    //PlayerProjectile
    SDL_Texture* playerProjTex = initTexture(renderer, "assets/round.png");
    //Enemy Textures
    SDL_Texture* enemyTex = initTexture(renderer, "assets/Scorpio.png");
    //Enemy Projectile
    SDL_Texture* enemyProjTex = initTexture(renderer, "assets/roundEnemy.png");
    //PlayerHealth
    SDL_Texture* healthTex = initTexture(renderer, "assets/heart.png");

    //Spawn hely, es alap ertekek
    int _hasQuit = 0;
    Player p = {150, 300, 1000, 1000, false, false, true, playerTex, false, 0, 0, 0};

    //Player lovedekei
    int iterGlobalPlayer = 0;
    Projectile* projArray = (Projectile*)malloc(10 * sizeof(Projectile));

    if (projArray == NULL)
    {
        printf("MEM ERR");
        exit(1);
    }

    //Ellensegek lovedekei
    int iterGlobalEnemy = 0;
    Projectile* enemyProjArray = (Projectile*)malloc(20 * sizeof(Projectile));
    if (enemyProjArray == NULL)
    {
        printf("MEM ERR");
        exit(1);
    }

    //ellensegek
    Enemy* enemies = (Enemy*)malloc(4 * sizeof(Enemy));
    if (enemies == NULL)
    {
        printf("MEM ERR");
        exit(1);
    }
    //Az y pozicio ahonnan kezdeni akarjuk a loop-ot.
    int k = 220;
    //Enemyket alaphelyzetbe rakjuk a match elejen
    srand(time(0));
    for (int i = 0; i < 4; i++)
    {
        enemies[i].x = 1150;
        enemies[i].y = k;
        enemies[i].lastShot = 0;
        enemies[i].rof = rand() % (1500 + 1 - 1250) + 1250;
        enemies[i].respawn = 0;
        enemies[i].health = 750;
        enemies[i].isDead = false;
        k += 80;
    }
    Game GameHandler = { renderer, &p, bgTex, playerProjTex, enemyTex, projArray, enemies, enemyProjArray, enemyProjTex, healthTex};

    //Event loop
    while (_hasQuit != 1)
    {
        //Event lekerdezese
        _hasQuit = eventProcess(window, &p);

        //Idle animacio vagy Setalo animacio?
        if (p.isWalking)
        {
            p.currentTex = playerWalkTex;
        }
        else
        {
            p.currentTex = playerTex;
        }
        //rendereljunk!
        doRender(&GameHandler, &iterGlobalPlayer, &iterGlobalEnemy);
        
        //Hogy ne legyen s�lt csirke a CPU-b�l...
        SDL_Delay(10);
    }


    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char fileName[21];
    sprintf(fileName, "score-%d-%02d-%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);


    scoreFile = fopen(fileName, "w");
    //ha sikerult letrehozni a fajlt...
    if (scoreFile != NULL)
    {
        fprintf(scoreFile, "Score - %d-%02d-%02d %02d:%02d:%02d\n---------------------------------------------\nSCORE: %f\nTotal distance travelled(units, not px): %d\nProjectiles shot: %d\nProjectiles landed: %d\nOverall accuracy: %.2f\n---------------------------------------------", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, ((double)GameHandler.p->projectilesLanded / (double)GameHandler.p->projectilesShot * GameHandler.p->score), GameHandler.p->totalDistance, GameHandler.p->projectilesShot, GameHandler.p->projectilesLanded, ((double)GameHandler.p->projectilesLanded / (double)GameHandler.p->projectilesShot * 100));
        fclose(scoreFile);
    }
    else
    {
        printf("ERR: nem sikerult megnyitni a fajlt!");
        exit(1);
    }


    /* ablak bezarasa */
    free(projArray);
    free(enemies);
    free(enemyProjArray);
    SDL_Quit();
    exit(0);

    return 0;
}