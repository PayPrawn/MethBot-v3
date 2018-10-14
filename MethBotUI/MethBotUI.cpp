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

	//renderer and window
	TTF_Init();
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = nullptr;
	SDL_Renderer *renderTarget = nullptr;
	window = SDL_CreateWindow("MethBotUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, xCoord, yCoord, SDL_WINDOW_SHOWN);
	renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//activation page textures
	SDL_Texture *loginBackground = nullptr;
	SDL_Texture *loginBackgroundtxtbx = nullptr;

	bool renderTxtbxText = false;

	//cursor
	SDL_Cursor* arrowCursor;
	SDL_Cursor* iBeamCursor;
	SDL_Cursor* handCursor;
	handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	iBeamCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

	//font
	SDL_Color black = { 0, 0, 0};
	SDL_Color grey = { 107, 107, 107 };
	SDL_Color white = { 255, 255, 255 };
	TTF_Font *font = nullptr;
	font = TTF_OpenFont("C:/Fonts/micross.ttf", 20);
	if (font == NULL) {
		std::cout << "Font Failure" << std::endl;
	}
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, "activation key", grey);
	if (textSurface == NULL) {
		std::cout << "Textsurface error" << std::endl;
	}
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderTarget, textSurface);
	if (text == NULL) {
		std::cout << "text error" << std::endl;
	}

	//activation page
	if (hasKey == 0) { //checks if key has been entered
		loginBackground = LoadTexture("Images/loginscreen.bmp", renderTarget); 
		loginBackgroundtxtbx = LoadTexture("Images/loginscreentextboxuse.bmp", renderTarget);
	}
	


	bool isRunning = true;
	SDL_Event ev;

	SDL_Rect textRect;
	textRect.x = 350;
	textRect.h = 10;
	textRect.w = 10;
	textRect.y = 350;
	SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);

	std::string inputText = "";
	SDL_StartTextInput();
	const char *txtbxText = "";

	while (isRunning)
	{

		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
				isRunning = false;
			else if (ev.button.x > 333 && ev.button.x < 772 && ev.button.y > 328 && ev.button.y < 396) {
				SDL_SetCursor(iBeamCursor);
				if (ev.type == SDL_MOUSEBUTTONDOWN) {
					std::cout << "text box click" << std::endl;
					renderTxtbxText = true;
				}
			}
			else if (ev.button.x > 810 && ev.button.x < 913 && ev.button.y > 328 && ev.button.y < 396) {
				SDL_SetCursor(handCursor);
				if (ev.type == SDL_MOUSEBUTTONDOWN) {
					std::cout << "confirm box click" << std::endl;
				}
			}
			else if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				renderTxtbxText = false;
			}
			else {
				SDL_SetCursor(arrowCursor);
			}
			if(renderTxtbxText)
				
				//special key input
				if (ev.type == SDL_TEXTINPUT || ev.type == SDL_KEYDOWN)
				{
					system("cls");
					//Handle backspace
					if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
					{
						inputText = inputText.substr(0, inputText.length() - 1);
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
					}
				//normal events
					else if (ev.type == SDL_TEXTINPUT)
						if(!((ev.text.text[0] == 'c' || ev.text.text[0] == 'C') && (ev.text.text[0] == 'v' || ev.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
						{
							inputText += ev.text.text;
						}
					std::cout << inputText << std::endl;
				}
		}
		SDL_RenderClear(renderTarget);
		if (renderTxtbxText) {
			SDL_RenderCopy(renderTarget, loginBackgroundtxtbx, NULL, NULL);
		}
		else {
			SDL_RenderCopy(renderTarget, loginBackground, NULL, NULL);
		}
		if (inputText.length() < 1 && renderTxtbxText == false) {
			SDL_RenderCopy(renderTarget, text, NULL, &textRect);
		}
		const char *txtbxText = inputText.c_str();
		SDL_Surface *textSurface2 = TTF_RenderText_Solid(font, txtbxText, grey);
		SDL_Texture *text2 = SDL_CreateTextureFromSurface(renderTarget, textSurface2);
		SDL_Rect textRectTxtBx;
		textRectTxtBx.x = 350;
		textRectTxtBx.h = 10;
		textRectTxtBx.w = 10;
		textRectTxtBx.y = 350;
		SDL_QueryTexture(text2, NULL, NULL, &textRectTxtBx.w, &textRectTxtBx.h);
		SDL_RenderCopy(renderTarget, text2, NULL, &textRectTxtBx);

		
		SDL_RenderPresent(renderTarget);
	}
	SDL_StopTextInput();

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(text);
	SDL_DestroyTexture(loginBackground);
	SDL_DestroyRenderer(renderTarget);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(loginBackgroundtxtbx);

	window = nullptr;
	renderTarget = nullptr;
	text = nullptr;
	loginBackground = nullptr;
	textSurface = nullptr;

	TTF_Quit();
	SDL_Quit();

	return 0;
}