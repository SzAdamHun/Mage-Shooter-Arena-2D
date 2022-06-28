#include "GameTypes.h"
#include "debugmalloc.h"
#pragma once
void renderHealth(Player* play, SDL_Renderer* renderer, SDL_Texture* healthTex) {
    int healthX = 50;
    for (int h = 1; h <= play->health / 100; h++)
    {
        SDL_Rect healthRect = { healthX, 660, 36, 36 };
        SDL_RenderCopy(renderer, healthTex, NULL, &healthRect);
        healthX += 48;
    }
}

/// <summary>
/// Transforms a given projectile shot by the player to the right, if it hits any of the enemies, then it changes the state of the enemy.
/// </summary>
/// <param name="p">Projectile component, the projectile that we want to transform.</param>
/// <param name="enemies">Array of enemies, function iterates through it.</param>
void transformPlayerProjectile(Projectile* p, Enemy* enemies, Player* player) {
    if (p->x <= 1250)
    {
        for (int i = 0; i < 4; i++)
        {
            if (sqrt(pow(enemies[i].x - p->x, 2) + pow(enemies[i].y - p->y + 44, 2)) <= 25 && !enemies[i].isDead)
            {
                player->projectilesLanded++;
                p->hasHit = true;
                p->x = 0;
                p->y = 0;
                //most fog meghalni!
                if (enemies[i].health == 250)
                {
                    player->score += 100;
                    //printf("%d\n", player->score);
                    enemies[i].isDead = true;
                    enemies[i].respawn = SDL_GetTicks();
                }
                else {
                    enemies[i].health -= 250;
                }
                return;
            }
        }
        p->x += p->speed;
    }
    else
    {
        p->hasHit = true;
        p->x = 0;
        p->y = 0;
    }
}

/// <summary>
/// Transforms a given projectile shot by the enemy to the left, if it hits the Player, then it changes the state of the Player (health).
/// </summary>
/// <param name="p"></param>
/// <param name="play"></param>
void transformEnemyProjectile(Projectile* p, Player* play) {
    if (p->x >= 30)
    {
        for (int i = 0; i < 20; i++)
        {
            if (sqrt(pow(play->x - p->x, 2) + pow(play->y - p->y + 44, 2)) - 26 <= 26 && !play->isDead) {
                p->hasHit = true;
                p->y = 0;
                p->x = 0;
                if (play->health == 100)
                {
                    play->isDead = true;
                }
                else
                {
                    play->health -= 100;
                }
                return;
            }
        }
        p->x -= p->speed;
    }
    else
    {
        p->y = 0;
        p->x = 0;
        p->hasHit = true;
    }
}
