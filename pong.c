#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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

typedef struct Player{
	int score;
	float yPosition;
} Player;

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 75;
const int PLAYER_MARGIN = BALL_SIZE;
const float PLAYER_MOVE_SPEED = 150.0f;

bool served = false;

Ball ball;

Player player1;
Player player2;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool Initialize(void);
void Update(float);
void Shutdown(void);
bool CoinFlip(void);

Ball MakeBall(int size);
void UpdateBall(Ball *ball, float elapsed);
void RenderBall(const Ball*);

Player MakePlayer(void);
void UpdatePlayers(float elapsed);
void RenderPlayers(void);

void UpdateScore(int player, int points);

int main(int argc, char* argv[]){
	srand((unsigned int)time(NULL));

	atexit(Shutdown); // causes the specified function func to be called when the program terminates.

	if(!Initialize()){
		exit(1);
	}		
	
	SDL_Event event;

	Uint32 lastTick = SDL_GetTicks(); // Get the number of miliseconds sice SDL Library initialization.
	while(1){
		if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
        }
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
	player1 = MakePlayer();
	player2 = MakePlayer();

	return true;
}

void Update(float elapsed){
	SDL_SetRenderDrawColor(renderer,0,0,0,0);
	SDL_RenderClear(renderer);

	UpdateBall(&ball, elapsed);
	RenderBall(&ball);

	UpdatePlayers(elapsed);
	RenderPlayers();

	SDL_RenderPresent(renderer);	// update the screen with any rendering performed since the previous call
}

void Shutdown(void){
	/* DESTROY*/
	if(renderer)
		SDL_DestroyRenderer(renderer);
	if(window)
		SDL_DestroyWindow(window);
	SDL_Quit();
}

bool CoinFlip(void){
	return rand() % 2 == 1 ? true : false;
}

Ball MakeBall(int size){
	const float SPEED = 120;
	Ball ball = {
		.x = WIDTH/2 - size/2,
		.y = HEIGHT/2 - size/2,
		.size = size,
		.xSpeed = SPEED * (CoinFlip() ? 1 : -1),
		.ySpeed = SPEED * (CoinFlip() ? 1 : -1),
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


void UpdateBall(Ball *ball, float elapsed){
	if(!served){
		ball->x = WIDTH/2;
		ball->y = HEIGHT/2;
		return;
	}

	ball->x += ball->xSpeed * elapsed;
	ball->y += ball->ySpeed * elapsed;
	if(ball->x < BALL_SIZE){
		//ball->xSpeed = fabs(ball->xSpeed);
		UpdateScore(2,100);
	}
	if(ball->x > WIDTH - BALL_SIZE){
		//ball->xSpeed = -fabs(ball->xSpeed);
		UpdateScore(1,100);
	}
	if(ball->y < BALL_SIZE){
		ball->ySpeed = fabs(ball->ySpeed);
	}
	if(ball->y > HEIGHT - BALL_SIZE){
		ball->ySpeed = -fabs(ball->ySpeed);
	}
}

Player MakePlayer(void){
	Player player = {
		.yPosition = HEIGHT/2,
	};
	return player;
}
void UpdatePlayers(float elapsed){
	const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
	if(keyboardState[SDL_SCANCODE_SPACE]){
		served = true;
	}
	if(keyboardState[SDL_SCANCODE_W]){
		player1.yPosition -= PLAYER_MOVE_SPEED * elapsed;
	}
	if(keyboardState[SDL_SCANCODE_S]){
		player1.yPosition += PLAYER_MOVE_SPEED * elapsed;
	}
	if(keyboardState[SDL_SCANCODE_UP]){
		player2.yPosition -= PLAYER_MOVE_SPEED * elapsed;
	}
	if(keyboardState[SDL_SCANCODE_DOWN]){
		player2.yPosition += PLAYER_MOVE_SPEED * elapsed;
	}

	if(player1.yPosition < PLAYER_HEIGHT/2 + BALL_SIZE){
		player1.yPosition = PLAYER_HEIGHT/2 + BALL_SIZE;
	}
	if(player1.yPosition > HEIGHT - PLAYER_HEIGHT/2 - BALL_SIZE){
		player1.yPosition = HEIGHT - PLAYER_HEIGHT/2 - BALL_SIZE;
	}
	if(player2.yPosition < PLAYER_HEIGHT/2 + BALL_SIZE){
		player2.yPosition = PLAYER_HEIGHT/2 + BALL_SIZE;
	}
	if(player2.yPosition > HEIGHT - PLAYER_HEIGHT/2 - BALL_SIZE){
		player2.yPosition = HEIGHT - PLAYER_HEIGHT/2 - BALL_SIZE;
	}

	SDL_Rect ballRect = {
		.x = ball.x - ball.size/2,
		.y = ball.y - ball.size/2,
		.w = ball.size,
		.h = ball.size,
	};

	SDL_Rect player1rect = {
		.x = PLAYER_MARGIN,
		.y = (int)(player1.yPosition)- PLAYER_HEIGHT/2,
		.w = PLAYER_WIDTH,
		.h = PLAYER_HEIGHT,
	};

	if(SDL_HasIntersection(&ballRect, &player1rect)){
		ball.xSpeed = fabs(ball.xSpeed);
	}

	SDL_Rect player2rect = {
		.x = WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
		.y = (int)(player2.yPosition)- PLAYER_HEIGHT/2,
		.w = PLAYER_WIDTH,
		.h = PLAYER_HEIGHT,
	};

	if(SDL_HasIntersection(&ballRect, &player2rect)){
		ball.xSpeed = -fabs(ball.xSpeed);
	}
}
void RenderPlayers(void){
	SDL_Rect player1rect = {
		.x = PLAYER_MARGIN,
		.y = (int)(player1.yPosition)- PLAYER_HEIGHT/2,
		.w = PLAYER_WIDTH,
		.h = PLAYER_HEIGHT,
	};
	SDL_SetRenderDrawColor(renderer,255,0,0,255);
	SDL_RenderFillRect(renderer, &player1rect);

	SDL_Rect player2rect = {
		.x = WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
		.y = (int)(player2.yPosition)- PLAYER_HEIGHT/2,
		.w = PLAYER_WIDTH,
		.h = PLAYER_HEIGHT,
	};
	SDL_SetRenderDrawColor(renderer,0,255,255,0);
	SDL_RenderFillRect(renderer, &player2rect);
}

void UpdateScore(int player, int points){
	served = false;
	if(player==1){
		player1.score += points;
	}
	if(player==2){
		player2.score += points;
	}
	char *fnt = "Player 1: %d | Player 2: %d";
	int len = snprintf(NULL, 0, fnt, player1.score, player2.score);
	char buf[len+1];
	snprintf(buf, len+1, fnt, player1.score, player2.score);
	SDL_SetWindowTitle(window, buf);
}
