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
void Pala::Init(int _player, SDL_Renderer *renderer){
    int _x;
	player = _player;

	// La posición de la pala va en función del jugador
	// Si el jugador es %2 == 0, entonces va la izquierda
	// en caso contrario va a la derecha

	if( (player%2) == 0){
        _x = PALA_WIDTH + (player/2)* PALA_WIDTH;
	}else{
        _x = WIN_WIDTH -2*PALA_WIDTH -PALA_WIDTH*(player/2);
	}

	rect.x = _x;
    rect.w = PALA_WIDTH;
    rect.h = PALA_HEIGHT;
    rect.y = WIN_HEIGHT/2;
    scrRect.x = 0;
    scrRect.w = PALA_WIDTH;
    scrRect.h = PALA_HEIGHT;
    scrRect.y = 0;
    SDL_Surface *imagenSur;
    switch(player){
        case 1:imagenSur = IMG_Load("assets/palas/player1.png");break;
        case 2:imagenSur = IMG_Load("assets/palas/player2.png");break;
        case 3:imagenSur = IMG_Load("assets/palas/player3.png");break;
        case 4:imagenSur = IMG_Load("assets/palas/player4.png");break;
        case 5:imagenSur = IMG_Load("assets/palas/player5.png");break;
        case 6:imagenSur = IMG_Load("assets/palas/player6.png");break;
        case 7:imagenSur = IMG_Load("assets/palas/player7.png");break;
        default:imagenSur = IMG_Load("assets/palas/player0.png");break;
    }
    imagen = SDL_CreateTextureFromSurface(renderer, imagenSur);
	speed = 0.5;
	angle = 0;


	//direcciones
	delta = 0;
}

//Update para la IA
void Pala::Update(float deltaTime, Direcction dir){

    if(dir == DIRECTION_UP){
        delta -= (float)deltaTime * speed * 1000;
    }else if(dir == DIRECTION_DOWN){
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

//render
void Pala::Render(SDL_Renderer* renderer){
    ///SDL_FillRect(surf,&rect,COLOR_PALA);
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, imagen, &scrRect, &rect, angle, NULL, flip);
}

SDL_Rect* Pala::getRect(){
    return &rect;
}
