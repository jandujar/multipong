#ifndef PALA_H
#define PALA_H

#include <SDL.h>
#include "SDL_net.h"
#include <SDL_image.h>

typedef enum { DIRECTION_UP = 1, DIRECTION_DOWN = 2, DIRECTION_NONE = 0 } Direcction;

class Pala
{
    public:
        Pala();
        virtual ~Pala();

        IPaddress ipaddress;

        //Initialization
        void Init(int player, SDL_Renderer *renderer);

        //Update para la IA
        void Update(float deltaTime);

        void SetIP(IPaddress _ip);
        //render
        void Render(SDL_Renderer* renderer);

        SDL_Rect* getRect();

        Direcction direccion_pala;
    protected:

    private:
        SDL_Rect rect;
        SDL_Rect scrRect;
        SDL_Texture *imagen;
        float speed;
        float delta;
        float angle;
        int player;
};

#endif // PALA_H
