#ifndef GAME_H
#define GAME_H


#include "Red.h"
#include "Pala.h"
#include "Bola.h"
#include "Tablero.h"
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
        void servidorEnviaDatos();


        Red red;
        SDL_Surface *sur;

        int numPlayers;
        int playerNumber;
        std::vector<Pala*> palas;
        Bola bola;
        Tablero tablero;

};

#endif // GAME_H
