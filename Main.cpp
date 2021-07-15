//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Elements constants
// Tank position
const int TANK_POS_X = 50;
const int TANK_POS_Y = 600;

// Global vars
long a, b, delta;
const int MAX_ELEMENTS = 4;

// Struct for TRON Player
struct gameElement {
	int xmin, xmax, ymin, ymax, active;
	SDL_Surface* psurface;
	SDL_Rect eltRect;

	// Constructor
	gameElement(SDL_Surface* pelement, int x, int y) {
		xmin = x;
		xmax = pelement->w + x;
		ymin = y;
		ymax = pelement->h + y;
		active = 1;
		psurface = pelement;
		eltRect = { x, y, 0, 0 };
	}

};

int main(int argc, char* args[])
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	// Event and Exit management
	int quit = 0;
	SDL_Event event;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Load sprites
	SDL_Surface* pBackground = SDL_LoadBMP("./resources/background.bmp");
	SDL_Surface* pBarrel = SDL_LoadBMP("./resources/barrel.bmp");
	SDL_Surface* pTank = SDL_LoadBMP("./resources/tank.bmp");
	SDL_Surface* pTurret = SDL_LoadBMP("./resources/turret.bmp");
	SDL_Surface* pBullet = SDL_LoadBMP("./resources/bullet.bmp");

	// Init game elements
	// Init gameElement (0 Tank, Others Barrels)
	gameElement gameElements[MAX_ELEMENTS] = { gameElement(pTank, 25, 535),
												gameElement(pBarrel, 565, 400),
												gameElement(pBarrel, 760, 230),
												gameElement(pBarrel, 1024, 475) };

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("SDL Tank", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{

			/* Loop until an SDL_QUIT event is found */
			while (!quit) {

				/* Poll for events */
				while (SDL_PollEvent(&event)) {

					switch (event.type) {
						/* Keyboard event */
						// TODO gestion des mouvements de la tourelle
					case SDL_KEYDOWN:
						/* Touche appuyée, changement de statut */
						switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							//alien_xvel = -1;
							break;
						case SDLK_RIGHT:
							//alien_xvel = 1;
							break;
						case SDLK_SPACE:
							//alien_yvel = 1;
							break;
						case SDLK_ESCAPE:
							quit = 1;
							break;
						default:
							break;
						}


						break;
						/* Lorsque la touche est relachée, on annule le mouvement */
					case SDL_KEYUP:

						/* Touche appuyée, changement de statut */
						switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							//alien_xvel = -1;
							break;
						case SDLK_RIGHT:
							//alien_xvel = 1;
							break;
						case SDLK_UP:
							//alien_yvel = -1;
							break;
						case SDLK_DOWN:
							//alien_yvel = 1;
							break;
						case SDLK_SPACE:
							//alien_yvel = 1;
							break;
						case SDLK_ESCAPE:
							quit = 1;
							break;
						default:
							break;
						}

						break;

						/* SDL_QUIT event (window close) */
					case SDL_QUIT:
						quit = 1;
						break;

					default:
						break;
					}

				}


				// Capping to 60 FPS
				a = SDL_GetTicks();
				delta = a - b;

				if (delta > 1000 / 60.0) {

					// FPS Management
					b = a;

					// TODO - Gestion des collisions
					// TODO - Gestion de la physique de l'obus

					//Get window surface
					screenSurface = SDL_GetWindowSurface(window);

					if (pBackground && pBarrel && pTank && pTurret && pBullet)
					{
						SDL_Rect backgroundDest = {0, 0, 0, 0 };

						// Dessin du décor
						SDL_BlitSurface(pBackground, NULL, screenSurface, &backgroundDest); // Dessin du fond de l'écran

						// Dessin du tank et des barils
						// Corps du tank + Barils
						for (int i = 0; i < MAX_ELEMENTS; i++) {
							SDL_BlitSurface(gameElements[i].psurface, NULL, screenSurface, &gameElements[i].eltRect); // Dessin de chaque élément
						}


						// Tourelle du tank
						// TODO

						// Dessin de l'obus
						// TODO

						SDL_UpdateWindowSurface(window); // Mise à jour de la fenêtre pour prendre en compte la copie du sprite
					}
					else
					{
						fprintf(stdout, "Échec de chargement du sprite (%s)\n", SDL_GetError());
					}

					//Fill the surface white
					//SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x11, 0x11, 0xDD));

					//Update the surface
					SDL_UpdateWindowSurface(window);



				}

			}

		}
	}

	// Libération de la mémoire pour les Sprites
	SDL_FreeSurface(pBackground);
	SDL_FreeSurface(pBarrel);
	SDL_FreeSurface(pTank);
	SDL_FreeSurface(pTurret);
	SDL_FreeSurface(pBullet);

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}