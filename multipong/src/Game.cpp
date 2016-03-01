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
    pala->Init(palas.size());
    IPaddress fakeaddress;
    fakeaddress.host=0;
    fakeaddress.port=0;
    pala->SetIP(fakeaddress);
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
    float senderTime = 0;

    while(!quit){
        currentTime = SDL_GetTicks();
        deltaTime = (float)(currentTime - lastTime) / 1000;
        lastTime = currentTime;

        senderTime+=deltaTime;

        //Recibo datos de los clientes
        red.servidorRecibeDatos(&palas, deltaTime, numPlayers);

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

        //Interpolo posici�n palas
        for(i = 0; i<(int)palas.size();i++){
            palas[i]->Update(deltaTime);
        }

        //Muevo Bola
        if(palas.size() == numPlayers){
            bola.Update(palas, deltaTime);
        }

        //Render de cosas
        bola.Render(sur);
        tablero.render(sur);
        for(i = 0; i<(int)palas.size();i++){
            palas[i]->Render(sur);
        }

        //Servidor envia los datos a todos los clientes
        if(senderTime>=SEND_TIME){
            senderTime-=SEND_TIME;
            servidorEnviaDatos();
        }

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
    float senderTime = 0;
    while(!quit){
        currentTime = SDL_GetTicks();
        deltaTime = (float)(currentTime - lastTime) / 1000;
        lastTime = currentTime;

        senderTime+=deltaTime;

        //Recibo los datos del servidor //sobreescribo los datos por defecto
        red.clienteRecibeDatos(&palas, &bola);


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

        //Interpolo posici�n palas
        for(i = 0; i<(int)palas.size();i++){
            palas[i]->Update(deltaTime);
        }

        //Interpolo posicion bolas
        if(palas.size()>0){
            bola.Update(palas, deltaTime);
        }


        //Render de cosas
        bola.Render(sur);
        tablero.render(sur);
        for(i = 0; i<(int)palas.size();i++){
            palas[i]->Render(sur);
        }

        //Servidor envia los datos a todos los clientes
        if(senderTime>=SEND_TIME){
            senderTime-=SEND_TIME;
            //Envio direccion al servidor
            red.clienteEnviaDireccion((int)dir);
        }


        SDL_UpdateWindowSurface(win);
        SDL_Delay(25);
    }
}

void Game::servidorEnviaDatos(){
    char datos_enviar[MAX_BUFFER];
    char tmp[MAX_BUFFER];

    if(palas.size() == 1){
        sprintf(datos_enviar,"%d %d %d %d %d %d\n",palas.size(),bola.getRect()->x,bola.getRect()->y,palas[0]->getRect()->x,palas[0]->getRect()->y, (int)palas[0]->direccion_pala);
    }else if(palas.size()==2){
        sprintf(datos_enviar,"%d %d %d %d %d %d %d %d %d\n",palas.size(),bola.getRect()->x,bola.getRect()->y,palas[0]->getRect()->x,palas[0]->getRect()->y,(int)palas[0]->direccion_pala, palas[1]->getRect()->x,palas[1]->getRect()->y, (int)palas[1]->direccion_pala);
    }else if(palas.size()==3){
        sprintf(datos_enviar,"%d %d %d %d %d %d %d %d %d %d %d %d\n",palas.size(),bola.getRect()->x,bola.getRect()->y,palas[0]->getRect()->x,palas[0]->getRect()->y,(int)palas[0]->direccion_pala, palas[1]->getRect()->x,palas[1]->getRect()->y, (int)palas[1]->direccion_pala,palas[2]->getRect()->x,palas[2]->getRect()->y, (int)palas[2]->direccion_pala);
    }else if(palas.size()==4){
        sprintf(datos_enviar,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",palas.size(),bola.getRect()->x,bola.getRect()->y,palas[0]->getRect()->x,palas[0]->getRect()->y,(int)palas[0]->direccion_pala, palas[1]->getRect()->x,palas[1]->getRect()->y, (int)palas[1]->direccion_pala, palas[2]->getRect()->x,palas[2]->getRect()->y, (int)palas[2]->direccion_pala, palas[3]->getRect()->x,palas[3]->getRect()->y, (int)palas[3]->direccion_pala);
    }
    red.servidorEnviaDatosATodos(&palas, datos_enviar);
}
