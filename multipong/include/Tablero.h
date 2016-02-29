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

        void init(int w);
        void render(SDL_Surface* surf);
    protected:

    private:
};

#endif // TABLERO_H
