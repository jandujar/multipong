#ifndef TABLERO_H
#define TABLERO_H

#include "Pala.h"

class Tablero
{
    public:
        Tablero();
        virtual ~Tablero();

        SDL_Rect midaTablero;
        SDL_Rect* getRect();

        void init(int w, SDL_Renderer *renderer);
        void render(SDL_Renderer* renderer);
    protected:

    private:
        SDL_Texture *imagen;
};

#endif // TABLERO_H
