#ifndef PALA_H
#define PALA_H

#include <SDL.h>
#include "SDL_net.h"

typedef enum { DIRECTION_UP = 1, DIRECTION_DOWN = 2, DIRECTION_NONE = 0 } Direcction;

class Pala
{
    public:
        Pala();
        virtual ~Pala();

        //Initialization
        void Init(int player);

        //Update para la IA
        void Update(float deltaTime, Direcction dir);

        void SetIP(IPaddress _ip);
        //render
        void Render(SDL_Surface* surf);

        SDL_Rect* getRect();
    protected:

    private:
        SDL_Rect rect;
        float speed;
        float delta;
        int player;
        IPaddress ipaddress;
};

#endif // PALA_H
