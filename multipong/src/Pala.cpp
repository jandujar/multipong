#include "Pala.h"
#include "Constants.h"
#include "math.h"

Pala::Pala()
{
    //ctor
}

Pala::~Pala()
{
    //dtor
}


//Initialization
void Pala::Init(int _player){
    int _x;
	player = _player;
	// La posición de la pala va en función del jugador
	// Si el jugador es %2 == 0, entonces va la izquierda
	// en caso contrario va a la derecha

	if( (player%2) == 0){
        _x = PALA_WIDTH + (2*player)* PALA_WIDTH;
	}else{
        _x = WIN_WIDTH -2*PALA_WIDTH -PALA_WIDTH*(2*player);
	}

	rect.x = _x;
    rect.w = PALA_WIDTH;
    rect.h = PALA_HEIGHT;
    rect.y = WIN_HEIGHT/2;

	speed = 0.5;


	//direcciones
	delta = 0;

	direccion_pala = DIRECTION_NONE;
}

//Update para la IA
void Pala::Update(float deltaTime){

    if(direccion_pala == DIRECTION_UP){
        delta -= (float)deltaTime * speed * 1000;
    }else if(direccion_pala == DIRECTION_DOWN){
        delta += (float)deltaTime * speed * 1000;
    }

    //Si te puedes mover un pixel, lo hace
    if (fabs(delta) >= 1){
        int aux = 0;
        if (delta > 0)
            aux = (int)(delta + 0.5f);
        else
            aux = (int)(delta - 0.5f);
        rect.y += aux;
        delta = 0;
    }

    //Si salimos por arriba o por abajo, recolocamos
    if(rect.y + rect.h > WIN_HEIGHT){
        rect.y = WIN_HEIGHT - rect.h;
    }else if(rect.y < 0 ){
        rect.y = 0;
    }

}

void Pala::SetIP(IPaddress _ip) {
    ipaddress = _ip;
}
//render
void Pala::Render(SDL_Surface* surf){
    SDL_FillRect(surf,&rect,COLOR_PALA);
}

SDL_Rect* Pala::getRect(){
    return &rect;
}
