#include "Game.h"
#include <stdio.h>

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

    //Definimos el número de jugadores
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

    //Esperamos el número de jugadores -1 (porque nosotros somos un jugador)
    //red.esperaClientes(numPlayers - 1, numPlayers, 1);

    //Inicio la bola
    bola.Init();

    bool quit = false;
    int lastTime = SDL_GetTicks();
    int currentTime = SDL_GetTicks();
    float deltaTime = 0;

    Direcction dir = DIRECTION_NONE;

    while(!quit){
        currentTime = SDL_GetTicks();
        deltaTime = (float)(currentTime - lastTime) / 1000;
        lastTime = currentTime;

        red.servidorRecibeDatos(palas, deltaTime);

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
        // palas[0]->Update(deltaTime,dir);




        //Muevo Bola
        bola.Update(palas, deltaTime);


        //Render de cosas
        bola.Render(sur);
        for(i = 0; i<(int)palas.size();i++){
            palas[i]->Render(sur);
        }

        //Servidor envia los datos a todos los clientes
        servidorEnviaDatos();

        //Recibo datos de los clientes
        red.servidorRecibeDatos(palas, deltaTime);

        SDL_UpdateWindowSurface(win);
        SDL_Delay(25);
    }
}


void Game::iniciaCliente(SDL_Window *win, std::string host, int port){
    sur = SDL_GetWindowSurface(win);

    //Iniciamos conexion de red
    red.inicia();

    red.iniciaCliente(host, port);

    //Recibo el número de jugadores y el player actual
    red.clienteRecibeNumeros(&numPlayers, &playerNumber);

    int i;
    //Creamos las palas en funcion del numero de jugadores
    for(i=0; i<numPlayers; i++){
        Pala *pala = new Pala();
        pala->Init(i);
        palas.push_back(pala);
    }

    //Inicio la bola
    bola.Init();

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
        if(red.clienteRecibeDatos(msg)>=0){
            clienteCargaDatos(msg);
        }


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
        //palas[0]->Update(deltaTime,dir);

        //Muevo Bola
        //bola.Update(palas, deltaTime);


        //Render de cosas
        bola.Render(sur);
        for(i = 0; i<numPlayers;i++){
            palas[i]->Render(sur);
        }

        //Envio direccion al servidor
        red.clienteEnviaDireccion(playerNumber, (int)dir);

        SDL_UpdateWindowSurface(win);
        SDL_Delay(25);
    }
}

void Game::clienteCargaDatos(char* msg){
    //Que cargamos? la posicion de la bola, la posición de los jugadores x jugadores
    sscanf(msg,"%d %d %d %d %d %d",&bola.getRect()->x,&bola.getRect()->y,&palas[0]->getRect()->x,&palas[0]->getRect()->y,&palas[1]->getRect()->x,&palas[1]->getRect()->y);
}

void Game::servidorEnviaDatos(){
    char datos_enviar[MAX_BUFFER];
    //sprintf(datos_enviar,"%d %d %d %d %d %d",bola.getRect()->x,bola.getRect()->y,palas[0]->getRect()->x,palas[0]->getRect()->y,palas[1]->getRect()->x,palas[1]->getRect()->y);

    //red.servidorEnviaDatosATodos(datos_enviar);
}
