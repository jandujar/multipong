#ifndef MARCADOR_H
#define MARCADOR_H

#include <SDL.h>
#include <SDL_image.h>

class Marcador
{
    public:
        Marcador();
        virtual ~Marcador();

        //Initialization
        void Init(int numEquipo, SDL_Renderer* renderer);

        //Update
        void Update();

        //render
        void Render(SDL_Renderer* renderer);

        void resetScore();
        void setScore(int _score);
        void incrementScore(int _increment);
        void updateScoreLeftRight();
        int findXScoreDigit(int _score);

        int getScore(){ return score; };
        int getScore_left(){ return score_left; };
        int getScore_right(){ return score_right; };
        int getX_score_left(){ return x_score_left; };
        int getX_score_right(){ return x_score_right; };

        //SDL_Rect* getRect();

    protected:

    private:
        SDL_Rect srcRect_left, srcRect_right;
        SDL_Rect dstRect_left, dstRect_right;
        SDL_Texture *texImagen;

        int team, score, score_left, score_right, x_score_left, x_score_right;
};

#endif // MARCADOR_H
