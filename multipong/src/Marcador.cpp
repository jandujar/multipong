#include "Marcador.h"
#include "Constants.h"

Marcador::Marcador()
{
    //ctor
}

Marcador::~Marcador()
{
    //dtor
}

void Marcador::Init(int numEquipo, SDL_Renderer* renderer)
{
    team = numEquipo;

    SDL_Surface *imagenSurf;
    imagenSurf = IMG_Load("assets/nums.png");
    texImagen = SDL_CreateTextureFromSurface(renderer, imagenSurf);

    if(team == 1)
    {
        dstRect_left.x = WIN_WIDTH/2 - 20 - MARCADOR_WIDTH;
        dstRect_right.x = WIN_WIDTH/2 - 20 - (MARCADOR_WIDTH/2);
    }
    else if(team == 2)
    {
        dstRect_left.x = WIN_WIDTH/2 + 20;
        dstRect_right.x = WIN_WIDTH/2 + 20 + (MARCADOR_WIDTH/2);
    }
    dstRect_left.y = 20;
    dstRect_right.y = 20;

	//size of 1 digit in sprite sheet png
    dstRect_left.w = 32;
    dstRect_right.w = 32;
    dstRect_left.h = 64;
    dstRect_right.h = 64;

    srcRect_left.x = 0;
    srcRect_left.y = 0;
    srcRect_left.w = dstRect_left.w;
    srcRect_left.h = dstRect_left.h;

    srcRect_right.x = 0;
    srcRect_right.y = 0;
    srcRect_right.w = dstRect_right.w;
    srcRect_right.h = dstRect_right.h;

	score = 0;
	score_left = 0;
	score_right = 0;
	x_score_left = 0;
	x_score_right = 0;
}

void Marcador::Update() {
}

//render
void Marcador::Render(SDL_Renderer* renderer){
	srcRect_left.x = getX_score_left();
	srcRect_left.y = 0;

    SDL_RenderCopy(renderer, texImagen, &srcRect_left, &dstRect_left);

    srcRect_right.x = getX_score_right();
	srcRect_right.y = 0;

    SDL_RenderCopy(renderer, texImagen, &srcRect_right, &dstRect_right);
}

/*SDL_Rect* Marcador::getRect(){
    return &rect;
}*/

void Marcador::resetScore(){
	score = 0;
	score_left = 0;
	score_right = 0;
	x_score_left = 0;
	x_score_right = 0;
}

void Marcador::setScore(int _score){
	score = _score;
	updateScoreLeftRight();
}

void Marcador::incrementScore(int _score){
	score += _score;
	updateScoreLeftRight();
}

void Marcador::updateScoreLeftRight() {
	score_left = (int)score / 10;
	score_right = (int)score % 10;

	x_score_left = findXScoreDigit(score_left);
	x_score_right = findXScoreDigit(score_right);
}

int Marcador::findXScoreDigit(int _score)
{
	int x_score = 0;

	switch (_score)
	{
	case 0:
		x_score = 0;
		break;
	case 1:
		x_score = 35;
		break;
	case 2:
		x_score = 65;
		break;
	case 3:
		x_score = 99;
		break;
	case 4:
		x_score = 131;
		break;
	case 5:
		x_score = 165;
		break;
	case 6:
		x_score = 197;
		break;
	case 7:
		x_score = 230;
		break;
	case 8:
		x_score = 260;
		break;
	case 9:
		x_score = 292;
		break;
	default:
		break;
	}

	return x_score;
}

