#include"stdafx.h"
#include<SDL.h>
#include<iostream>
#include<vector>
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
#include<map>
//////////////////////////////////////////////////////////variables//////////////////////////////////////////////////////////////////////////
int xCoord = 1280;
int yCoord = 720;
int hasKey = 0;
const char *key = "d";
bool renderTxtbxText = false;
bool buttonClick = false;
bool failed = false;
bool isRunning = true;
const char *txtbxText = "";
std::string inputText = "";
SDL_Event ev;
SDL_Cursor* arrowCursor;
SDL_Cursor* iBeamCursor;
SDL_Cursor* handCursor;
struct DrawTex {
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
};
struct Button {
	std::map<std::string,SDL_Texture*> texture;
	SDL_Rect Rect;
	std::string tag;
};
std::vector<Button>Buttons;
std::string pageOn = "Login";
TTF_Font *font = nullptr;
SDL_Color black;
SDL_Color grey;
SDL_Color white;
SDL_Color red;
SDL_Texture *loginBackground = nullptr;
SDL_Texture *loginBackgroundtxtbx = nullptr;
SDL_Texture *loginBackgroundClick = nullptr;
SDL_Texture *text2 = nullptr;
SDL_Surface *textSurface2 = nullptr;
SDL_Surface *textSurface3 = nullptr;
SDL_Texture *text3 = nullptr;
SDL_Texture *text = nullptr;
SDL_Surface *textSurface = nullptr;
SDL_Window *window = nullptr;
SDL_Renderer *renderTarget = nullptr;
SDL_Texture *menuBackground = nullptr;

Button SUPREME;
Button PALACE;

SDL_Rect textRect;
SDL_Rect textRectTxtBx;
SDL_Rect failedRect;
/////////////////////////////////////////////////////////functions///////////////////////////////////////////////////////////////////////////

bool checkclick(SDL_Rect button, int x = 0, int y = 0) {
	if (x >= button.x &&
		x <= button.x + button.w &&
		y >= button.y &&
		y <= button.y + button.h
		)
	{
		return true;
	}
	else {
		return false;
	}
	
};

void evHandler()
{
	arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	iBeamCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	while (SDL_PollEvent(&ev) != 0)
	{
		if (ev.type == SDL_QUIT)
			isRunning = false;
		if (pageOn == "Login") {

			if (ev.button.x > 333 && ev.button.x < 772 && ev.button.y > 328 && ev.button.y < 396) {
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
					buttonClick = true;

				}
			}
			else if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				renderTxtbxText = false;
			}
			else {
				SDL_SetCursor(arrowCursor);
			}
			if ((renderTxtbxText && inputText.length() < 16) || (renderTxtbxText && ev.key.keysym.sym == SDLK_BACKSPACE) || (renderTxtbxText && ev.key.keysym.sym == SDLK_RETURN)) {

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
					else if (ev.key.keysym.sym == SDLK_RETURN) {
						buttonClick = true;
					}
					//normal events
					else if (ev.type == SDL_TEXTINPUT) {
						if (!((ev.text.text[0] == 'c' || ev.text.text[0] == 'C') && (ev.text.text[0] == 'v' || ev.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
						{
							inputText += ev.text.text;
						}

					}
					std::cout << inputText << std::endl;

				}
			}

		}
		else if (pageOn == "Mainmenu") {
			bool hovering = false;
			for (auto i : Buttons) {
				if (checkclick(i.Rect, ev.button.x, ev.button.y)) {
					hovering = true;
				}
			}
			if (hovering) {
				SDL_SetCursor(handCursor);
			}
			else {
				SDL_SetCursor(arrowCursor);
			}
			for (auto i : Buttons) {
				if (checkclick(i.Rect, ev.button.x, ev.button.y)) {
					if (ev.type == SDL_MOUSEBUTTONDOWN) {
						if (i.tag == "SUPREME") {
							SUPREME.texture["CURRENT"] = SUPREME.texture["CLICKED"];

						}
					}	
					if (ev.type == SDL_MOUSEBUTTONUP) {
						if (i.tag == "SUPREME") {
							SUPREME.texture["CURRENT"] = SUPREME.texture["DEFAULT"];

						}
					}
				}
			}	
		}
	}
}
//renderer and window
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

void renderHandler() {
	SDL_RenderClear(renderTarget);
	if (pageOn == "Login"){

		if (renderTxtbxText) {
			SDL_RenderCopy(renderTarget, loginBackgroundtxtbx, NULL, NULL);
			if (buttonClick) {
				SDL_RenderCopy(renderTarget, loginBackgroundClick, NULL, NULL);
				SDL_Delay(100);
				renderTxtbxText = false;
			}
		}
		else {
			SDL_RenderCopy(renderTarget, loginBackground, NULL, NULL);
		}
		if (inputText.length() < 1 && renderTxtbxText == false) {
			SDL_RenderCopy(renderTarget, text, NULL, &textRect);
		}
		if (failed) {
			SDL_RenderCopy(renderTarget, text3, NULL, &failedRect);
		}
		SDL_DestroyTexture(text2);
		txtbxText = inputText.c_str();
		textSurface2 = TTF_RenderText_Solid(font, txtbxText, grey);
		text2 = SDL_CreateTextureFromSurface(renderTarget, textSurface2);

		SDL_QueryTexture(text2, NULL, NULL, &textRectTxtBx.w, &textRectTxtBx.h);
		SDL_RenderCopy(renderTarget, text2, NULL, &textRectTxtBx);
	}
	if (pageOn == "Mainmenu") {
		SDL_RenderCopy(renderTarget, menuBackground, NULL,NULL);

		SDL_RenderCopy(renderTarget, SUPREME.texture["CURRENT"], NULL, &SUPREME.Rect);
	}

}


int main(int argc, char *argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 framestart;
	int frametime;
	//renderer and window
	TTF_Init();
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("MethBotUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, xCoord, yCoord, SDL_WINDOW_SHOWN);
	renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Buttons
	SUPREME.Rect.x = 0;
	SUPREME.Rect.y = 0;
	SUPREME.Rect.w = 400;
	SUPREME.Rect.h = 138;
	SUPREME.tag = "SUPREME";
	SUPREME.texture["CLICKED"] = LoadTexture("Images/Mainmenu/Supreme Logo Clicked.bmp", renderTarget);
	SUPREME.texture["DEFAULT"] = LoadTexture("Images/Mainmenu/Supreme Logo.bmp", renderTarget);
	SUPREME.texture["CURRENT"] = SUPREME.texture["DEFAULT"];
	Buttons.emplace_back(SUPREME);

	menuBackground = LoadTexture("Images/Mainmenu/Background.bmp",renderTarget);
	if (menuBackground != NULL) {
		std::cout << "MENU LOADED " << std::endl;
	}
	//cursor
	//font
    black = { 0, 0, 0};
    grey = { 107, 107, 107 };
    white = { 255, 255, 255 };
    red = { 255, 0, 0 };
    font = nullptr;
	font = TTF_OpenFont("C:/Fonts/micross.ttf", 32);
	if (font == NULL) {
		std::cout << "Font Failure" << std::endl;
	}
	textSurface = TTF_RenderText_Solid(font, "activation key", grey);
	if (textSurface == NULL) {
		std::cout << "Textsurface error" << std::endl;
	}
	text = SDL_CreateTextureFromSurface(renderTarget, textSurface);
	if (text == NULL) {
		std::cout << "text error" << std::endl;
	}
	textSurface3 = TTF_RenderText_Solid(font, "invalid key", red);
	text3 = SDL_CreateTextureFromSurface(renderTarget, textSurface3);

	//activation page textures
	if (hasKey == 0) { //checks if key has been entered
		loginBackground = LoadTexture("Images/loginscreen.bmp", renderTarget); 
		loginBackgroundtxtbx = LoadTexture("Images/loginscreentextboxuse.bmp", renderTarget);
		loginBackgroundClick = LoadTexture("Images/loginbackgroundclick.bmp", renderTarget);
		
	}
	



	textRect.x = 350;
	textRect.h = 10;
	textRect.w = 10;
	textRect.y = 342;
	SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);

	textRectTxtBx.x = 350;
	textRectTxtBx.h = 10;
	textRectTxtBx.w = 10;
	textRectTxtBx.y = 342;

	failedRect.x = 465;	
	failedRect.w = 10;
	failedRect.h = 10;
	failedRect.y = 270;
	SDL_QueryTexture(text3, NULL, NULL, &failedRect.w, &failedRect.h);

	
	SDL_StartTextInput();

	while (isRunning)
	{
		framestart = SDL_GetTicks();
		evHandler();
	
		renderHandler();

		SDL_RenderPresent(renderTarget);
		if (buttonClick) {
			if (strcmp(txtbxText, key) == 0) {
				//Activation succesful
				failed = false;
				pageOn = "Mainmenu";
			}
			else {
				failed = true;
			}
			SDL_Delay(100);
			buttonClick = false;
		}
		frametime = SDL_GetTicks() - framestart;
		if (frameDelay > frametime) {
			SDL_Delay(frameDelay - frametime);
		}
	}
	SDL_StopTextInput();
	SDL_FreeSurface(textSurface2);
	SDL_DestroyTexture(text2);
	SDL_RenderClear(renderTarget);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(text);
	SDL_DestroyTexture(loginBackground);
	SDL_DestroyRenderer(renderTarget);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(loginBackgroundtxtbx);
	SDL_DestroyTexture(loginBackgroundClick);
	

	window = nullptr;
	renderTarget = nullptr;
	text = nullptr;
	loginBackground = nullptr;
	textSurface = nullptr;

	TTF_Quit();
	SDL_Quit();

	return 0;
	}

