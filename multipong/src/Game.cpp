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
    ///sur = SDL_GetWindowSurface(win);
    _gRenderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    //Iniciamos conexion de red
    red.inicia();

    //Definimos el número de jugadores
    numPlayers = _numberPlayers;

    //Iniciamos el servidor
    red.iniciaServidor(port);

    int i;
    //Creamos las palas en funcion del numero de jugadores
    //for(i=0; i<numPlayers; i++){
    //    Pala *pala = new Pala();
    //    pala->Init(i);
    //    palas.push_back(pala);
    //}
    // Crep la pala para el servidor
    Pala *pala = new Pala();
    pala->Init(palas.size(),_gRenderer);
    IPaddress fakeaddress;
    fakeaddress.host=0;
    fakeaddress.port=0;
    pala->SetIP(fakeaddress);
    std::cout << "Pala size " << palas.size() <<std::endl;
    palas.push_back(pala);
    std::cout << "Pala size " << palas.size() <<std::endl;

    //Esperamos el número de jugadores -1 (porque nosotros somos un jugador)
    //red.esperaClientes(numPlayers - 1, numPlayers, 1);

    //Inicio la bola
    bola.Init(_gRenderer);
    //Inicio el tablero
    tablero.init(5, _gRenderer);
    //Inicio los marcadores
    marcador1 = new Marcador();
    marcador1->Init(1, _gRenderer);
    marcador2 = new Marcador();
    marcador2->Init(2, _gRenderer);

    bool quit = false;
    int lastTime = SDL_GetTicks();
    int currentTime = SDL_GetTicks();
    float deltaTime = 0;

    Direcction dir = DIRECTION_NONE;
    float senderTime = 0;

    while(!quit){
        currentTime = SDL_GetTicks();
        deltaTime = (float)(currentTime - lastTime) / 1000;
        lastTime = currentTime;

        senderTime+=deltaTime;

        //Recibo datos de los clientes
        red.servidorRecibeDatos(&palas, deltaTime, numPlayers, _gRenderer);

        //Inicio surface
        ///SDL_FillRect(sur,NULL,0);
        SDL_SetRenderDrawColor(_gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(_gRenderer);



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
                    palas[0]->direccion_pala = DIRECTION_UP;
                }
                else if(tecla == SDL_SCANCODE_DOWN){
                    palas[0]->direccion_pala = DIRECTION_DOWN;
                }
                break;
            case SDL_KEYUP:
                tecla = test_event.key.keysym.scancode;
                if (tecla == SDL_SCANCODE_ESCAPE){
                    quit = true;
                }
                else if(tecla == SDL_SCANCODE_UP){
                    palas[0]->direccion_pala = DIRECTION_NONE;
                }
                else if(tecla == SDL_SCANCODE_DOWN){
                    palas[0]->direccion_pala = DIRECTION_NONE;
                }
                break;

            case SDL_QUIT:
                quit = true;
                break;
            }
        }

        //Interpolo posición palas
        for(i = 0; i<(int)palas.size();i++){
            palas[i]->Update(deltaTime);
        }

        //Muevo Bola
        if(palas.size() == numPlayers){
			bola.Update(palas, marcador1, marcador2, deltaTime);
        }

        //Render de cosas
        tablero.render(_gRenderer);
        bola.Render(_gRenderer);
        for(i = 0; i<numPlayers;i++){
            palas[i]->Render(_gRenderer);
        }
        marcador1->Render(_gRenderer);
        marcador2->Render(_gRenderer);

        //Servidor envia los datos a todos los clientes
        if(senderTime>=SEND_TIME){
            senderTime-=SEND_TIME;
            servidorEnviaDatos();
        }

        ///SDL_UpdateWindowSurface(win);
        SDL_RenderPresent(_gRenderer);
        SDL_Delay(25);
    }
}


void Game::iniciaCliente(SDL_Window *win, std::string host, int port){
    ///sur = SDL_GetWindowSurface(win);
    _gRenderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    //Iniciamos conexion de red
    red.inicia();

    red.iniciaCliente(host, port);

    //Recibo el número de jugadores y el player actual
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
    bola.Init(_gRenderer);
    //Inicio el tablero
    tablero.init(5, _gRenderer);
    //Inicio los marcadores
    marcador1 = new Marcador();
    marcador1->Init(1, _gRenderer);
    marcador2 = new Marcador();
    marcador2->Init(2, _gRenderer);

    bool quit = false;
    int lastTime = SDL_GetTicks();
    int currentTime = SDL_GetTicks();
    char msg[MAX_BUFFER];
    float deltaTime = 0;

    Direcction dir = DIRECTION_NONE;
    float senderTime = 0;
    while(!quit){
        currentTime = SDL_GetTicks();
        deltaTime = (float)(currentTime - lastTime) / 1000;
        lastTime = currentTime;

        senderTime+=deltaTime;

        //Recibo los datos del servidor //sobreescribo los datos por defecto
        red.clienteRecibeDatos(&palas, &bola,_gRenderer);


        //Inicio surface
        ///SDL_FillRect(sur,NULL,0);
        SDL_SetRenderDrawColor(_gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(_gRenderer);


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

        //Interpolo posición palas
        for(i = 0; i<(int)palas.size();i++){
            palas[i]->Update(deltaTime);
        }

        //Interpolo posicion bolas
        if(palas.size()>0){
            bola.Update(palas, marcador1, marcador2, deltaTime);
        }


        //Render de cosas
        tablero.render(_gRenderer);
        bola.Render(_gRenderer);
        for(i = 0; i<(int)palas.size();i++){
            palas[i]->Render(_gRenderer);
        }
        marcador1->Render(_gRenderer);
        marcador2->Render(_gRenderer);

        //Servidor envia los datos a todos los clientes
        if(senderTime>=SEND_TIME){
            senderTime-=SEND_TIME;
            //Envio direccion al servidor
            red.clienteEnviaDireccion((int)dir);
        }


        ///SDL_UpdateWindowSurface(win);
        SDL_RenderPresent(_gRenderer);
        SDL_Delay(25);
    }
}

void Game::servidorEnviaDatos(){
    char datos_enviar[MAX_BUFFER];
    char tmp[MAX_BUFFER];

    if(palas.size() == 1){
        sprintf(datos_enviar,"%d %d %d %d %d %d %d %d\n",palas.size(),marcador1->getScore(), marcador2->getScore(), bola.getRect()->x,bola.getRect()->y,palas[0]->getRect()->x,palas[0]->getRect()->y, (int)palas[0]->direccion_pala);
    }else if(palas.size()==2){
        sprintf(datos_enviar,"%d %d %d %d %d %d %d %d %d %d %d\n",palas.size(),marcador1->getScore(), marcador2->getScore(), bola.getRect()->x,bola.getRect()->y,palas[0]->getRect()->x,palas[0]->getRect()->y,(int)palas[0]->direccion_pala, palas[1]->getRect()->x,palas[1]->getRect()->y, (int)palas[1]->direccion_pala);
    }else if(palas.size()==3){
        sprintf(datos_enviar,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",palas.size(),marcador1->getScore(), marcador2->getScore(), bola.getRect()->x,bola.getRect()->y,palas[0]->getRect()->x,palas[0]->getRect()->y,(int)palas[0]->direccion_pala, palas[1]->getRect()->x,palas[1]->getRect()->y, (int)palas[1]->direccion_pala,palas[2]->getRect()->x,palas[2]->getRect()->y, (int)palas[2]->direccion_pala);
    }else if(palas.size()==4){
        sprintf(datos_enviar,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",palas.size(),marcador1->getScore(), marcador2->getScore(), bola.getRect()->x,bola.getRect()->y,palas[0]->getRect()->x,palas[0]->getRect()->y,(int)palas[0]->direccion_pala, palas[1]->getRect()->x,palas[1]->getRect()->y, (int)palas[1]->direccion_pala, palas[2]->getRect()->x,palas[2]->getRect()->y, (int)palas[2]->direccion_pala, palas[3]->getRect()->x,palas[3]->getRect()->y, (int)palas[3]->direccion_pala);
    }
    red.servidorEnviaDatosATodos(&palas, datos_enviar);
}
/*
void Game::clienteCargaDatos(char* msg){
    int score1 = 0, score2 = 0;
    //Que cargamos? la posicion de la bola, la posición de los jugadores x jugadores y la linea central
    sscanf(msg,"%d %d %d %d %d %d %d %d",&bola.getRect()->x,&bola.getRect()->y,&palas[0]->getRect()->x,&palas[0]->getRect()->y,&palas[1]->getRect()->x,&palas[1]->getRect()->y, &score1, &score2);

    marcador1->setScore(score1);
    marcador2->setScore(score2);
}
*/
