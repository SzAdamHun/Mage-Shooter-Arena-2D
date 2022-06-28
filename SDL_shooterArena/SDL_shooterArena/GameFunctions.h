#include "PlayerUtils.h"
#include "debugmalloc.h"
/// <summary>
/// This function makes a texture out of a .png give from a set directory.
/// </summary>
/// <param name="renderer">Renderer component, texture creation depends on it.</param>
/// <param name="f">File directory string, based on directory of the project (E. g. "assets/shoot.png")</param>
/// <returns>Texture made out of a png.</returns>
SDL_Texture* initTexture(SDL_Renderer* renderer, const char* f) {
    SDL_Surface* temp = IMG_Load(f);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    SDL_RenderClear(renderer);
    return tex;
}

/// <summary>
/// Processes the the current input(s), then returns either 0 or 1 based on current player state and/or input.
/// </summary>
/// <param name="window">Window instance, we need to process input events through a window component.</param>
/// <param name="player">Player instance, the function changes player state based on input.</param>
/// <returns>1 if game is over, 0 otherwise.</returns>
int eventProcess(SDL_Window* window, Player* player) {
    SDL_Event eve;
    int end = 0;
    if (player->isDead)
    {
        return 1;
    }
    while (SDL_PollEvent(&eve))
    {
        switch (eve.type) {
        case SDL_QUIT:
            end = 1;
            break;
        case SDL_KEYDOWN:
            switch (eve.key.keysym.sym) {
            case SDLK_ESCAPE:
                end = 1;
                break;
            default:
                break;
            }
        default:
            break;
        case SDL_MOUSEBUTTONDOWN:
            player->hasShot = true;
            break;
        }

    }
    //Smoothabb event handling mintha switchbe raknank.
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_A])
    {
        player->x -= 4;
        player->totalDistance += 4;
        player->isWalking = true;
    }
    else if (state[SDL_SCANCODE_D])
    {
        player->x += 4;
        player->totalDistance += 4;
        player->isWalking = true;

    }
    else if (state[SDL_SCANCODE_W] && player->y >= 250)
    {
        player->y -= 4;
        player->totalDistance += 4;
        player->isWalking = true;
    }
    else if (state[SDL_SCANCODE_S] && player->y <= 480)
    {
        player->y += 4;
        player->totalDistance += 4;
        player->isWalking = true;
    }
    else
    {
        player->isWalking = false;
    }

    return end;
}

/// <summary>
/// Renders all game components based on current game state
/// </summary>
/// <param name="g">Game instance, for parameters, see parameters of "Game".</param>
/// <param name="iter">Iterator for the player's projectiles, has to be global to be used between renders.</param>
/// <param name="enemyIter">Iterator for the enemies' projectiles, has to be global to be used between renders.</param>
void doRender(Game* g, int* iter, int* enemyIter) {

    Uint32 ticks = SDL_GetTicks();
    Uint32 seconds;

    // Ha setalunk akkor gyorsabban akarjuk hogy jobban passzoljon az animacio
    // sebessege a karakter sebessegehez.
    if (g->p->isWalking)
    {
        seconds = ticks / 100;
    }
    else
    {
        seconds = ticks / 500;
    }
    Uint32 sprite = seconds % 4;
    SDL_Rect SrcRect = { sprite * 48, 0, 48, 48 };
    SDL_Rect destRect = { g->p->x, g->p->y, 96, 96 };


    if (g->p->hasShot)
    {
        //ha lohetunk
        if (ticks - g->p->lastShot >= 500)
        {
            g->p->projectilesShot++;
            g->projectiles[*iter].x = g->p->x;
            g->projectiles[*iter].y = g->p->y;
            g->projectiles[*iter].hasHit = false;
            g->projectiles[*iter].Tex = g->playerProjTex;
            g->projectiles[*iter].speed = 5;
            g->p->lastShot = ticks;
            *iter = *iter == 9 ? 0 : *iter + 1;
        }
        g->p->hasShot = false;
    }

    //Hatter renderelese
    //Megj.: kell a hatteret is rendererlni frame-enkent, mert karakter, projectile, stb. renderelesehez folymatosan torolnunk kell
    //a kepernyon es a friss adatokkal rendererlni, es ugy nem lenne hatter.
    SDL_RenderClear(g->renderer);
    SDL_RenderCopy(g->renderer, g->bgT, NULL, NULL);
    SDL_RenderCopy(g->renderer, g->p->currentTex, &SrcRect, &destRect);


    renderHealth(g->p, g->renderer, g->healthTex);

    //enemyk renderelese
    for (int i = 0; i < 4; i++)
    {
        //... de csak ha nem halott!
        if (!g->enemies[i].isDead)
        {
            SDL_Rect enemyRect = { g->enemies[i].x, g->enemies[i].y, 96, 96 };
            SDL_RenderCopy(g->renderer, g->enemyTex, NULL, &enemyRect);
            if (ticks - g->enemies[i].lastShot >= g->enemies[i].rof)
            {
                g->enemyProjectiles[*enemyIter].x = g->enemies[i].x;
                g->enemyProjectiles[*enemyIter].y = g->enemies[i].y+25;
                g->enemyProjectiles[*enemyIter].hasHit = false;
                g->enemyProjectiles[*enemyIter].Tex = g->enemyProjTex;
                g->enemyProjectiles[*enemyIter].speed = rand() % (4 + 1 - 3) + 3;
                g->enemies[i].lastShot = ticks;
                *enemyIter = *enemyIter == 19 ? 0 : *enemyIter + 1;
            }
        }
        else
        {
            //Ha mar tobb mint 5 masodperce halott, akkor kovetkezo rendernel mar eljen, respawnoljon!
            if (ticks - g->enemies[i].respawn >= 5000)
            {
                g->enemies[i].isDead = false;
                g->enemies[i].health = 750;
                g->enemies[i].rof = rand() % (1500 + 1 - 1250) + 1250;
            }
        }
    }

    //Player lovedekek eltolasa
    for (int i = 0; i < 10; i++)
    {
        if (&g->projectiles[i] != NULL)
        {
            transformPlayerProjectile(&g->projectiles[i], g->enemies, g->p);
            if (!g->projectiles[i].hasHit)
            {
                SDL_Rect projRect = { (g->projectiles[i]).x, (g->projectiles[i]).y, 54, 54 };
                SDL_RenderCopy(g->renderer, g->playerProjTex, NULL, &projRect);
            }
        }
    }

    //enemy lovedekek eltolasa
    for (int l = 0; l < 20; l++)
    {
        if (&g->enemyProjectiles[l] != NULL)
        {
            transformEnemyProjectile(&g->enemyProjectiles[l], g->p);
            if (!g->enemyProjectiles[l].hasHit)
            {
                SDL_Rect enemyProjRect = { (g->enemyProjectiles[l].x), (g->enemyProjectiles[l].y),54, 54 };
                SDL_RenderCopy(g->renderer, g->enemyProjTex, NULL, &enemyProjRect);
            }
        }
    }

    SDL_RenderPresent(g->renderer);
}