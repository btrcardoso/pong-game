#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int WIDTH = 640;
const int HEIGHT = 480;
const int BALL_SIZE = 10;

typedef struct Ball {
	float x;
	float y;
	float xSpeed;
	float ySpeed;
	int size;
} Ball;
Ball ball;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *bitmapSurface = NULL; // contains a collection of pixels used in software blitting
SDL_Texture *bitmapTex = NULL;     // contains an specific represetantion of pixel data

bool Initialize(void);
void Update(float);
void Shutdown(void);
Ball MakeBall(int size);
void RenderBall(const Ball*);

int main(int argc, char* argv[]){
	//srand((unsigned int)time(NULL));

	atexit(Shutdown); // causes the specified function func to be called when the program terminates.

	if(!Initialize()){
		exit(1);
	}		
	
	/* QUIT */
	SDL_Event event;
	while(SDL_WaitEvent(&event)){   // read the event until to be SDL_QUIT, then it quit.
		if(event.type == SDL_QUIT){
			break;
		}
		Uint32 lastTick = SDL_GetTicks(); // Get the number of miliseconds sice SDL Library initialization.
		Uint32 curTick = SDL_GetTicks();
		Uint32 diff = curTick - lastTick;
		float elapsed = diff / 1000.0f;
		Update(elapsed);
		lastTick = curTick;
	}
	SDL_Quit();
}


bool Initialize(void){
	/* INIT */
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)!=0){
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	/* WINDOW */
	window = SDL_CreateWindow(
		"Pong",                  // title
		SDL_WINDOWPOS_UNDEFINED, // x position
		SDL_WINDOWPOS_UNDEFINED, // y position
		WIDTH,					 // width
		HEIGHT,                  // height
		SDL_WINDOW_OPENGL        // flags
	);
	if(!window){
		printf("Couldn't create window %s\n", SDL_GetError());
		return false;
	}
    
    /* RENDERER */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // SDL_Renderer: a structure that contains a rendering state. SDL_CreateRender: create a 2D rendering context for a window
	if(!window){
		printf("Couldn't create window %s\n", SDL_GetError());
		return false;
	}

	ball = MakeBall(BALL_SIZE);

	return true;
}

void Update(float elapsed){
	/*PUTTING IMAGE*/
	bitmapSurface = SDL_LoadBMP("img/sky.bmp");                   
	bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
	SDL_FreeSurface(bitmapSurface);                                    // free a surface
	SDL_RenderClear(renderer);                                         // clear the current rendering target with the drawing color
	SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);                   // copy a portion of the texture to the current rendering target

	/*BALL*/
	RenderBall(&ball);

	SDL_RenderPresent(renderer);	// update the screen with any rendering performed since the previous call
}

void Shutdown(void){
	/* DESTROY*/
	if(bitmapTex)
		SDL_DestroyTexture(bitmapTex);
	if(renderer)
		SDL_DestroyRenderer(renderer);
	if(window)
		SDL_DestroyWindow(window);
	SDL_Quit();
}

bool CoinFLip(void){
	return rand() % 2 == 1 ? true : false;
}

Ball MakeBall(int size){
	const float SPEED = 120;
	Ball ball = {
		.x = WIDTH/2 - size/2,
		.y = HEIGHT/2 - size/2,
		.size = size,
		.xSpeed = SPEED * (CoinFLip() ? 1 : -1),
		.ySpeed = SPEED * (CoinFLip() ? 1 : -1),
	};
	return ball;
}
 
void RenderBall(const Ball *ball){
	int size = ball->size;
	int halfSize = size/2;
	SDL_Rect rect = {
		.x = ball->x - halfSize,
		.y = ball->y - halfSize,
		.w = size,
		.h = size,
	};
	SDL_SetRenderDrawColor(renderer, 127,127,127,127);
	SDL_RenderFillRect(renderer, &rect);       // fill a rectangle on the current rendering target with the drawing color.
}
