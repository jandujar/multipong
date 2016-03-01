#ifndef GAME_H
#define GAME_H

#include <SDL_image.h>
#include "Red.h"
#include "Pala.h"
#include "Bola.h"
#include "Tablero.h"
#include "Marcador.h"
#include <vector>


class Game
{
    public:
        Game();
        virtual ~Game();

        void iniciaServidorJugador(SDL_Window *win, int numberPlayers, int port);
        void iniciaCliente(SDL_Window *win, std::string host, int port);
    protected:

    private:
        void clienteCargaDatos(char* msg);
        void servidorEnviaDatos();


        Red red;
        ///SDL_Surface *sur;
        SDL_Renderer *_gRenderer;

        int numPlayers;
        int playerNumber;
        std::vector<Pala*> palas;
        Bola bola;
        Tablero tablero;
        Marcador *marcador1;
        Marcador *marcador2;

};

#endif // GAME_H
