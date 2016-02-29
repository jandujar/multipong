#include "Bola.h"
#include <stdlib.h>
#include <math.h>
#include "Constants.h"
#include <iostream>


Bola::Bola()
{
    //ctor
}

Bola::~Bola()
{
    //dtor
    closeMedia();
}

//Initialization
void Bola::Init(){
    rect.h = BOLA_SIZE;
    rect.w = BOLA_SIZE;
    rect.y = WIN_HEIGHT/2;
    rect.x = WIN_WIDTH/2;
	//Iniciar con un angulo random, pero que nunca sea completamente horizontal o vertical
	int type = rand() % 4;
	switch (type)
	{
	case 0:	//15-75
		angle = rand() % 60 + 15;
		break;
	case 1: //105-165
		angle = rand() % 60 + 105;
		break;
	case 2: //195-255
		angle = rand() % 60 + 195;
		break;
	case 3://285-345
		angle = rand() % 60 + 285;
		break;
	}


	dx = dy = 0.0f;
	speed = 0.4f;
	rebotandoY = rebotandoX =  false;

	speedX = (float)cos(angle*M_PI / 180.0f) * speed;
	speedY = (float)sin(angle*M_PI / 180.0f) * speed;

	MAX_speed = speed *2;

	InitMedia();
}

void Bola::InitMedia(){
    //The music that will be played
    gMusic = NULL;

    //The sound effects that will be used
    gGol = NULL;
    gColision = NULL;
    gNewPj = NULL;

	//Initialize SDL_mixer
    if( SDL_Init( SDL_INIT_AUDIO ) < 0 ){
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
            std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    if(loadMedia()){
        //Play the music
        //if(Mix_PlayMusic( gMusic, -1 ))
          //  std::cout << "Musica carregada" << std::endl;
    }
}

//Update para la IA
void Bola::Update(std::vector<Pala*>palas, float deltaTime){
    //Si no esta ya rebotando (efecto bullet), rebota contra la pared
    if (!rebotandoY){
        if (rect.y + rect.h >= WIN_HEIGHT || rect.y <= 0){
            //nueva direccion
            angle = 360 -angle;
            speedX = (float)cos(angle*M_PI / 180.0f) * speed;
            speedY = (float)sin(angle*M_PI / 180.0f) * speed;
            rebotandoY = true;

            //
        }
    }

    //Si no esta ya rebotando (efecto bullet), rebota contra la tabla
    for(unsigned int i=0; i < palas.size(); i++){
            Pala* pala = palas[i];

            //Choca con la tabla
            if ((rect.x + rect.w >=pala->getRect()->x && rect.x <= (pala->getRect()->x + pala->getRect()->w)) && (rect.y >= pala->getRect()->y && rect.y + rect.h <= pala->getRect()->y + pala->getRect()->h)){

                if(rebotandoX)
                    break;
                //Numero del player (impar izquierda, par derecha)
                if((i+1)%2 != 0){
                    //Calculo el punto de interseccion con la tabla, y obtengo el angulo y la nueva velocidad
                //Cuanto mas al centro de la bola, mas recta y mas lenta sale
                    int intersection = (pala->getRect()->y + (pala->getRect()->h / 2)) - (rect.y + (rect.h / 2));
                    intersection /= (pala->getRect()->h / 2);	//[-1,1]
                    int newAngle = abs(intersection * 65) + 10;	//nunca horizontal!
                    speed += (abs(intersection)  * (MAX_speed - speed)) + speed * 0.05f;


                    if (angle <= 180){

                        angle = newAngle;
                    }
                    else{

                        angle = 360 - newAngle;
                    }

                }
                else{
                    int intersection = (pala->getRect()->y + (pala->getRect()->h / 2)) - (rect.y + (rect.h / 2));
                    intersection /= (pala->getRect()->h / 2);
                    int newAngle = abs(intersection * 65) + 10;	//nunca horizontal!
                    //speed = (abs(intersection) + 0.2) * MAX_speed;
                    speed += (abs(intersection)  * (MAX_speed - speed)) + speed * 0.05f;




                    if (angle <= 90){

                        angle = 180 - newAngle;
                    }
                    else{

                        angle = 180 + newAngle;
                    }
                }

                Rebote();
            }else rebotandoX = false;
        }


    //Actualiza posiciones
    dx += deltaTime * speedX *1000;
    dy += deltaTime * speedY * 1000;

    //Si se ha acumulado un pixel de movimiento, se mueve
    if (fabs(dx) >= 1){
        int aux = 0;
        if (dx > 0)
            aux = (int)(dx + 0.5f);
        else
            aux = (int)(dx - 0.5f);
        rect.x += aux;
        dx = 0;
        //Ya no choca contra nada?

    }
    //Si se ha acumulado un pixel de movimiento, se mueve
    if (fabs(dy) >= 1){

        int aux = 0;
        if (dy > 0)
            aux = (int)(dy + 0.5f);
        else
            aux = (int)(dy - 0.5f);
        rect.y += aux;
        dy = 0;
        //Ya no choca contra nada?
        if (rebotandoY && rect.y + rect.h < WIN_WIDTH && rect.y > 0)
            rebotandoY = false;
        //Comprueba si sigue chocando contra un bloque
    }

    if(rect.x < 0 ){
        std::cout << "Punto Equipo 2" << std::endl;
        Gol();
    }else if(rect.x > WIN_WIDTH){
        std::cout << "Punto Equipo 1" << std::endl;
        Gol();
    }
}


void Bola::Gol(){
    //Se inicia la bola en el centro y se cambia el angulo de salida

    rect.y = WIN_HEIGHT/2;
    rect.x = WIN_WIDTH/2;

    dx = dy = 0.0f;
	speed = 0.4f;

	//std::cout << "Angle = " << angle << std::endl;
	//std::cout << "SpeedX = " << speedX << std::endl;

	//nueva direccion
    angle = (angle +180) % 360;
    speedX = (float)cos(angle*M_PI / 180.0f) * speed ;
    speedY = (float)sin(angle*M_PI / 180.0f) * speed ;

    rebotandoY = rebotandoX =  false;

    //Play sound effect.
    Mix_PlayChannel( -1, gGol, 0 );
}


//render
void Bola::Render(SDL_Surface* surf){
    SDL_FillRect(surf,&rect,COLOR_BOLA);
}

SDL_Rect* Bola::getRect(){
    return &rect;
}

void Bola::Rebote(){
	//Incremento de la velocidad cada vez que rebota!
	MAX_speed += MAX_speed *0.05f;

	speedX = (float)cos(angle*M_PI / 180.0f) * speed;
	speedY = (float)sin(angle*M_PI / 180.0f) * speed;
	rebotandoX = true;

	//Play sound effect.
	Mix_PlayChannel( -1, gColision, 0 );
}

bool Bola::loadMedia(){
    //Loading success flag
    bool success = true;

    //Load music
    //gMusic = Mix_LoadMUS( "assets/sounds/musicPong.wav" );
    if( gMusic == NULL ){
        std::cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        success = false;
    }

    //Load sound effects
    gGol = Mix_LoadWAV( "assets/sounds/goalBall.wav" );
    if( gGol == NULL ){
        std::cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        success = false;
    }

    gColision = Mix_LoadWAV( "assets/sounds/colisionBall.wav" );
    if( gColision == NULL ){
        std::cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        success = false;
    }

    gNewPj = Mix_LoadWAV( "assets/sounds/newPlayer.wav" );
    if( gNewPj == NULL ){
        std::cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        success = false;
    }

    return success;
}

void Bola::closeMedia(){
    //Free the sound effects
    Mix_FreeChunk( gGol );
    Mix_FreeChunk( gColision );
    Mix_FreeChunk( gNewPj );
    gGol = NULL;
    gColision = NULL;
    gNewPj = NULL;

    //Free the music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;

    //Quit SDL subsystems
    Mix_Quit();
    SDL_Quit();
}
