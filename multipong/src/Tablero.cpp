#include "Tablero.h"
#include <stdlib.h>
#include <math.h>
#include "Constants.h"
#include <iostream>

Tablero::Tablero()
{
    //ctor
}

Tablero::~Tablero()
{
    //dtor
}

void Tablero::init(int w, SDL_Renderer *renderer)
{
    midaTablero.x = 0;
    midaTablero.y = 0;
    midaTablero.h = WIN_HEIGHT;
    midaTablero.w = WIN_WIDTH;

    SDL_Surface * surf = IMG_Load("assets/campo.png");
    imagen = SDL_CreateTextureFromSurface(renderer, surf);
}

void Tablero::render(SDL_Renderer* renderer)
{
    ///SDL_FillRect(sur,&midaTablero,0xFFFFFF);
    SDL_RenderCopy(renderer, imagen, &midaTablero, &midaTablero);
}
