#ifndef BOLA_H
#define BOLA_H

#include "Pala.h"
#include "Marcador.h"
#include <vector>
#include <SDL_mixer.h>

class Bola
{
    public:
        Bola();
        virtual ~Bola();

        //Initialization
        void Init(SDL_Renderer *renderer);
        void InitMedia();

        //Update para la IA
        void Update(std::vector<Pala*>palas, Marcador *marcador1, Marcador *marcador2, float deltaTime);

        //render
        void Render(SDL_Renderer* renderer);

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
        SDL_Texture *imagen;
        //cantidad de movimiento en x,y
        float dx, dy;

        bool rebotandoY, rebotandoX;

        float MAX_speed;
};

#endif // BOLA_H
