#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "Red.h"
#include "Game.h"
#include "Constants.h"

int main(int argc, char* argv[]){

    bool somosServidor = false;
    bool valoresDef = false;
    int jugadores = 0;
    int i = 0;
    std::string host = "";
    int puerto;

    if(argc == 2)
    {
        std::cout << "Argumentos 1" << std::endl;
        if(strcmp(argv[1], "-s") == 0){

            somosServidor = true;
            valoresDef = true;
        }
        else if(strcmp(argv[1], "-c") == 0)
        {
            valoresDef = true;
            somosServidor = false;
        }
        else return -1;
    }

    else if(argc < 3)
    {
        return -1;

    }
    else if(argc == 3)
    {
        while(i < argc)
        {
            if(strcmp(argv[i], "-s") == 0){
                i++;
                somosServidor = true;
            }
            else if(strcmp(argv[i], "-j") == 0)
            {
                int tempJug;

                if(sscanf(argv[i+1], "%d", &tempJug) == 1)
                {
                    if(tempJug > 1 && tempJug < 11)
                    {
                        jugadores = tempJug;
                        i+=2;
                    }
                    else
                    {
                    std::cout << "los jugadores estan mal" << std::endl;
                    return -1;
                    }
                }
            }
            else if(strcmp(argv[i], "-c") == 0)
            {
                host = argv[i+1];
                somosServidor = false;
                i+=2;
            }else if(strcmp(argv[i], "-p") == 0)
            {
                int tempPort;
                sscanf(argv[i+1], "%d", &tempPort);
                if(tempPort != -1)
                {
                    puerto = tempPort;
                }
                i+=2;
            }
            else{
                i++;
            }
        }
    }

    if(valoresDef){
        jugadores = 2;
        host = "localhost";
        puerto = 9999;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        return -1;
    }
    char TITLE[1024];

    SDL_Window *win = SDL_CreateWindow(TITLE, 100, 100, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }else{
        std::cout << "Ventana Inicializada" << std::endl;
    }

    //Creamos clase que se encargará del juego
    Game game;

    //si somos servidor
    if(somosServidor){
        game.iniciaServidorJugador(win, jugadores, 9999);
    }else{
        game.iniciaCliente(win, host, puerto);
    }


    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
