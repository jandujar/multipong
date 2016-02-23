#include "Game.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

Game::Game()
{
    //ctor
}

Game::~Game()
{
    //dtor
}


void Game::iniciaServidorJugador(SDL_Window *win, int _numberPlayers, int port){
    sur = SDL_GetWindowSurface(win);

    //Iniciamos conexion de red
    red.inicia();

    //Definimos el n�mero de jugadores
    numPlayers = _numberPlayers;

    //Iniciamos el servidor
    red.iniciaServidor(9999);

    int i;
    //Creamos las palas en funcion del numero de jugadores
    //for(i=0; i<numPlayers; i++){
    //    Pala *pala = new Pala();
    //    pala->Init(i);
    //    palas.push_back(pala);
    //}
    // Crep la pala para el servidor
    Pala *pala = new Pala();
    pala->Init(palas.size());
    std::cout << "Pala size " << palas.size() <<std::endl;
    palas.push_back(pala);
    std::cout << "Pala size " << palas.size() <<std::endl;

    //Esperamos el n�mero de jugadores -1 (porque nosotros somos un jugador)
    //red.esperaClientes(numPlayers - 1, numPlayers, 1);

    //Inicio la bola
    bola.Init();
    //Inicio el tablero
    tablero.init(5);

    bool quit = false;
    int lastTime = SDL_GetTicks();
    int currentTime = SDL_GetTicks();
    float deltaTime = 0;

    Direcction dir = DIRECTION_NONE;

    while(!quit){
        currentTime = SDL_GetTicks();
        deltaTime = (float)(currentTime - lastTime) / 1000;
        lastTime = currentTime;

        //Recibo datos de los clientes
        red.servidorRecibeDatos(palas, deltaTime, numPlayers);

        //Inicio surface
        SDL_FillRect(sur,NULL,0);


        SDL_Event test_event;
        SDL_Scancode tecla;
        while (SDL_PollEvent(&test_event)) {
            switch (test_event.type) {
            case SDL_KEYDOWN:
                tecla = test_event.key.keysym.scancode;
                if (tecla == SDL_SCANCODE_ESCAPE){
                    quit = true;
                }
                else if(tecla == SDL_SCANCODE_UP){
                    dir = DIRECTION_UP;
                }
                else if(tecla == SDL_SCANCODE_DOWN){
                    dir = DIRECTION_DOWN;
                }
                break;
            case SDL_KEYUP:
                tecla = test_event.key.keysym.scancode;
                if (tecla == SDL_SCANCODE_ESCAPE){
                    quit = true;
                }
                else if(tecla == SDL_SCANCODE_UP){
                    dir = DIRECTION_NONE;
                }
                else if(tecla == SDL_SCANCODE_DOWN){
                    dir = DIRECTION_NONE;
                }
                break;

            case SDL_QUIT:
                quit = true;
                break;
            }
        }

        //Muevo pala local (la del servidor)
        palas[0]->Update(deltaTime,dir);

        //Muevo Bola
        bola.Update(palas, deltaTime);

        //Render de cosas
        bola.Render(sur);
        tablero.render(sur);
        for(i = 0; i<(int)palas.size();i++){
            palas[i]->Render(sur);
        }

        //Servidor envia los datos a todos los clientes
        servidorEnviaDatos();

        SDL_UpdateWindowSurface(win);
        SDL_Delay(25);
    }
}


void Game::iniciaCliente(SDL_Window *win, std::string host, int port){
    sur = SDL_GetWindowSurface(win);

    //Iniciamos conexion de red
    red.inicia();

    red.iniciaCliente(host, port);

    //Recibo el n�mero de jugadores y el player actual
    //red.clienteRecibeNumeros(&numPlayers, &playerNumber);

    int i;
    // Crep la pala para el cliente
    /*
    Pala *pala = new Pala();
    pala->Init(palas.size());
    std::cout << "Pala size " << palas.size() <<std::endl;
    palas.push_back(pala);
    std::cout << "Pala size " << palas.size() <<std::endl;
    */

    //Inicio la bola
    bola.Init();
    //Inicio el tablero
    tablero.init(5);

    bool quit = false;
    int lastTime = SDL_GetTicks();
    int currentTime = SDL_GetTicks();
    char msg[MAX_BUFFER];
    float deltaTime = 0;

    Direcction dir = DIRECTION_NONE;

    while(!quit){
        currentTime = SDL_GetTicks();
        deltaTime = (float)(currentTime - lastTime) / 1000;
        lastTime = currentTime;

        //Recibo los datos del servidor
        //if(red.clienteRecibeDatos(msg)>=0){
        //    clienteCargaDatos(msg);
        //}


        //Inicio surface
        SDL_FillRect(sur,NULL,0);


        SDL_Event test_event;
        SDL_Scancode tecla;
        while (SDL_PollEvent(&test_event)) {
            switch (test_event.type) {
            case SDL_KEYDOWN:
                tecla = test_event.key.keysym.scancode;
                if (tecla == SDL_SCANCODE_ESCAPE){
                    quit = true;
                }
                else if(tecla == SDL_SCANCODE_UP){
                    dir = DIRECTION_UP;
                }
                else if(tecla == SDL_SCANCODE_DOWN){
                    dir = DIRECTION_DOWN;
                }
                break;
            case SDL_KEYUP:
                tecla = test_event.key.keysym.scancode;
                if (tecla == SDL_SCANCODE_ESCAPE){
                    quit = true;
                }
                else if(tecla == SDL_SCANCODE_UP){
                    dir = DIRECTION_NONE;
                }
                else if(tecla == SDL_SCANCODE_DOWN){
                    dir = DIRECTION_NONE;
                }
                break;

            case SDL_QUIT:
                quit = true;
                break;
            }
        }


        //Muevo pala local (la del cliente)
        //palas[0]->Update(deltaTime,dir);

        //Muevo Bola
        //bola.Update(palas, deltaTime);


        //Render de cosas
        bola.Render(sur);
        tablero.render(sur);
        for(i = 0; i<(int)palas.size();i++){
            palas[i]->Render(sur);
        }

        //Envio direccion al servidor
        red.clienteEnviaDireccion((int)dir);

        SDL_UpdateWindowSurface(win);
        SDL_Delay(25);
    }
}

void Game::clienteCargaDatos(char* msg){
    //Que cargamos? la posicion de la bola, la posici�n de los jugadores x jugadores y la linea central
    sscanf(msg,"%d %d %d %d %d %d",&bola.getRect()->x,&bola.getRect()->y,&palas[0]->getRect()->x,&palas[0]->getRect()->y,&palas[1]->getRect()->x,&palas[1]->getRect()->y);
}

void Game::servidorEnviaDatos(){
    char datos_enviar[MAX_BUFFER];
    //sprintf(datos_enviar,"%d %d %d %d %d %d",bola.getRect()->x,bola.getRect()->y,palas[0]->getRect()->x,palas[0]->getRect()->y,palas[1]->getRect()->x,palas[1]->getRect()->y);

    //red.servidorEnviaDatosATodos(datos_enviar);
}
