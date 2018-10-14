#include"stdafx.h"
#include<SDL.h>
#include<iostream>
#include"SDL_syswm.h"
#include"SDL_system.h"
#include<string>
#include<SDL_ttf.h>
#include"SDL_ttf.h"
#include<Windows.h>
#include<io.h>
#include<process.h>
#include<stdlib.h>
#include<fcntl.h>
SDL_Texture *LoadTexture(std::string filepath, SDL_Renderer *renderTarget)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *surface = SDL_LoadBMP(filepath.c_str());
	if (surface == NULL)
		std::cout << "Error" << std::endl;
	else
	{
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		if (texture == NULL)
			std::cout << "Error" << std::endl;
	}
	
	SDL_FreeSurface(surface);

	return texture;
}


int main(int argc, char *argv[])
{
	int xCoord = 1280;
	int yCoord = 720;

	int hasKey = 0;

	TTF_Init();
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = nullptr;
	SDL_Renderer *renderTarget = nullptr;
	window = SDL_CreateWindow("MethBotUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, xCoord, yCoord, SDL_WINDOW_SHOWN);
	renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


	SDL_Texture *currentImage = nullptr;
	SDL_Color white = { 255, 255, 255};
	TTF_Font *font = nullptr;
	font = TTF_OpenFont("C:/Fonts/micross.ttf", 20);
	if (font == NULL) {
		std::cout << "Font Failure" << std::endl;
	}
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, "This is a test of TTF", white);
	if (textSurface == NULL) {
		std::cout << "Textsurface error" << std::endl;
	}
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderTarget, textSurface);
	if (text == NULL) {
		std::cout << "text error" << std::endl;
	}


	

	currentImage = LoadTexture("Images/Log_in_screen_v1.bmp", renderTarget);


	bool isRunning = true;
	SDL_Event ev;

	SDL_Rect textRect;
	textRect.x = 200;
	textRect.h = 10;
	textRect.w = 10;
	textRect.y = 300;
	SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);

	std::string inputText = "";
	SDL_StartTextInput();


	while (isRunning)
	{
		bool renderText = false;

		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
				isRunning = false;

			//special key input
			else if (ev.type == SDL_TEXTINPUT || ev.type == SDL_KEYDOWN)
			{
				system("cls");
				//Handle backspace
				if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
				{
					inputText = inputText.substr(0, inputText.length() - 1);
					renderText = true;
				}
				//Handle copy
				else if (ev.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
				{
					SDL_SetClipboardText(inputText.c_str());
				}
				//Handle paste
				else if (ev.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
				{
					inputText = SDL_GetClipboardText();
					renderText = true;
				}
				//normal events
				else if (ev.type == SDL_TEXTINPUT)
					if(!((ev.text.text[0] == 'c' || ev.text.text[0] == 'C') && (ev.text.text[0] == 'v' || ev.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
				{
					inputText += ev.text.text;
					renderText = true;
				}
				std::cout << inputText << std::endl;
			}
		}
		SDL_RenderClear(renderTarget);
		
		SDL_RenderCopy(renderTarget, currentImage, NULL, NULL);
		SDL_RenderCopy(renderTarget, text, NULL, &textRect);
		SDL_RenderPresent(renderTarget);
	}
	SDL_StopTextInput();

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(text);
	SDL_DestroyTexture(currentImage);
	SDL_DestroyRenderer(renderTarget);
	SDL_DestroyWindow(window);

	window = nullptr;
	renderTarget = nullptr;
	text = nullptr;
	currentImage = nullptr;
	textSurface = nullptr;

	TTF_Quit();
	SDL_Quit();

	return 0;
}