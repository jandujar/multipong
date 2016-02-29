#include "Input.h"
#include <SDL.h>
#include <iostream>

Input::Input()
{
    //ctor
    arriba = false;
    abajo = false;
    exit = false;
}

Input::~Input()
{
    //dtor
}

void Input::Update(){
    SDL_Event event;

    while(SDL_PollEvent(&event)){ }

    const Uint8* current_key_states = SDL_GetKeyboardState( NULL );
    if( current_key_states[ SDL_SCANCODE_UP ] ) {
        arriba = true;
        std::cout << "Arriba" << std::endl;
    }else{
        arriba = false;
    }

    if( current_key_states[ SDL_SCANCODE_DOWN ] ) {
        abajo = true;
        std::cout << "Abajo" << std::endl;
    }else{
        abajo = false;
    }

    if( current_key_states[ SDL_SCANCODE_ESCAPE ] ) {
        exit = true;
        std::cout << "Exit" << std::endl;
    }

}
