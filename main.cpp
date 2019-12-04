/*
 * main.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: serg
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <ctime>

SDL_Texture* Back = NULL;
SDL_Texture* Symb = NULL;

SDL_Renderer* Render = NULL;

const int MaxSpriteSymb = 10;
int aReelSymb[5][5];

const int Xbase = 51;
const int Ybase = 90 - 130;
const int YendReel = 480;
const int YbegReel = 90;

bool ProgramIsRunning(); //event process
SDL_Texture* LoadTexture(char* filename);

void FreeFiles();
void DrawImageFrame(SDL_Texture* image,
		int x, int y, int width, int height, int frame);
void DrawImage(SDL_Texture* image, int x, int y);

int main(int argc, char* args[])
{
	int X=0, Y=0;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Failed to initialize SDL!\n");
		return 0;
	}

	srand(time(NULL));

	SDL_Window* window = SDL_CreateWindow("SDL!!!",
											SDL_WINDOWPOS_UNDEFINED,
											SDL_WINDOWPOS_UNDEFINED,
											800, 600, SDL_WINDOW_FULLSCREEN);

	Render = SDL_CreateRenderer(window, -1,
									SDL_RENDERER_ACCELERATED |
									SDL_RENDERER_PRESENTVSYNC);

	Back = LoadTexture("/home/serg/workspace/test3/res/backfon.png");
	Symb = LoadTexture("/home/serg/workspace/test3/res/reel.png");

	for(int i=0; i < 5; i++)
		for(int j=0; j < 5; j++)
			aReelSymb[i][j] = rand()%10;

	X = Xbase ; Y = Ybase;

	while(ProgramIsRunning())
	{
		if(Y >= 90){
			Y = Ybase;
			for(int i=4; i>0; i--)
				for(int j=0; j<5; j++)
					aReelSymb[i][j] = aReelSymb[i-1][j];
			for(int i=0; i<5; i++)
				aReelSymb[0][i]  = rand()%10;
		}

		DrawImage(Back,  0, 0);
		for(int row=0; row < 5; row++)
			for(int colmn=0; colmn < 5; colmn++)
				DrawImageFrame(Symb, X + 142*colmn, Y + 130*row, 130, 130, aReelSymb[row][colmn]);

		Y += 5;

//		SDL_Delay(20);
//		SDL_UpdateWindowSurface(window);
		SDL_RenderPresent(Render);

	}
//	FreeFiles();
	SDL_DestroyRenderer(Render);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void DrawImage(SDL_Texture* image,  int x, int y)
{
	SDL_Rect destRect;
	destRect.x = x;
	destRect.y = y;
//	SDL_BlitSurface(image, NULL, destSurface, &destRect);
	SDL_RenderCopy(Render, image, NULL, NULL);
}

void DrawImageFrame(SDL_Texture* image,
		int x, int y, int width, int height, int frame)
{

	int begDifpos = (y < YbegReel) ? (y+height)-YbegReel : 0;
	int begDifneg = height - begDifpos;

	SDL_Rect destRect;
	destRect.x = x;
	destRect.y = y + (begDifpos?begDifneg:0);
	destRect.h = (y < YbegReel) ? begDifpos :
			((y+height) > YendReel) ?
			((YendReel - y)<0 ? 0 : YendReel - y) : height;;
	destRect.w = 130;

	int columns = 10;

	SDL_Rect sourceRect;
	sourceRect.y = (begDifpos ? begDifneg : 0);
	sourceRect.x = (frame%columns)*width ;
	sourceRect.w = width;
	sourceRect.h =  (y < YbegReel) ? begDifpos :
					((y+height) > YendReel) ?
					((YendReel - y)<0 ? 0 : YendReel - y) : height;

//	SDL_BlitSurface(image, &sourceRect, destSurface, &destRect);
	SDL_RenderCopy(Render, image, &sourceRect , &destRect);
}

SDL_Texture* LoadTexture(char* filename )
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = NULL;

	surface =  IMG_Load(filename);

	if(surface != NULL)
	{
	//Let’s color key the surface
//		Uint32 colorKey = IMG_Load(surface->format, 255, 0, 255);
//		SDL_SetColorKey(surface, SDL_TRUE, colorKey);
		texture = SDL_CreateTextureFromSurface(Render, surface);
		SDL_FreeSurface(surface);
	}

	if(texture == NULL)
	{

	}
	return texture;
}

bool ProgramIsRunning()
{
	SDL_Event event;
	bool running = true;

	while(SDL_PollEvent(&event))
	{
		switch(event.type){
			case SDL_QUIT:	 {
				running = false;
				break;
			}
			case SDL_KEYDOWN: {
				running = false;
				break;
			}
		}
//		if(event.type == SDL_QUIT)
//			running = false;
	}
	return running;
}



