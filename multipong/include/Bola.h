#ifndef BOLA_H
#define BOLA_H

#include "Pala.h"
#include <vector>
#include <SDL_mixer.h>

class Bola
{
    public:
        Bola();
        virtual ~Bola();

        //Initialization
        void Init();
        void InitMedia();

        //Update para la IA
        void Update(std::vector<Pala*>palas, float deltaTime);

        //render
        void Render(SDL_Surface* surf);

        SDL_Rect* getRect();
    protected:

    private:
        void Gol();
        void Rebote();

        // Mixer.
        Mix_Music *gMusic;
        Mix_Chunk *gGol, *gColision, *gNewPj;
        bool loadMedia();
        void closeMedia();

        SDL_Rect rect;
        int angle;
        float speed, speedX, speedY;

        //cantidad de movimiento en x,y
        float dx, dy;

        bool rebotandoY, rebotandoX;

        float MAX_speed;
};

#endif // BOLA_H
