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
const SDL_Point turretCenter = { 25, 25 };
double turretAngle = 0;
double turretSpeed = 1;

// Global vars
long a, b, delta;
const int MAX_ELEMENTS = 4;

// Struct for TRON Player
struct gameElement {
	int xmin, xmax, ymin, ymax, active;
	SDL_Texture* texture;
	SDL_Rect eltRect;

	// Constructor
	gameElement(SDL_Surface* pelement, SDL_Texture* telement, int x, int y) {
		xmin = x;
		xmax = pelement->w + x;
		ymin = y;
		ymax = pelement->h + y;
		active = 1;
		texture = telement;
		eltRect = { x, y, pelement->w, pelement->h };
	}

};

int main(int argc, char* args[])
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

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




	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("SDL Tank", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, 0);

		// Init textures
		SDL_Texture* tBackground = SDL_CreateTextureFromSurface(renderer, pBackground);
		SDL_Texture* tBarrel = SDL_CreateTextureFromSurface(renderer, pBarrel);
		SDL_Texture* tTank = SDL_CreateTextureFromSurface(renderer, pTank);
		SDL_Texture* tTurret = SDL_CreateTextureFromSurface(renderer, pTurret);
		SDL_Texture* tBullet = SDL_CreateTextureFromSurface(renderer, pBullet);

		// Init game elements
		// Init gameElement (0 Tank, Others Barrels)
		gameElement gameElements[MAX_ELEMENTS] = { gameElement(pTank, tTank, 25, 535),
													gameElement(pBarrel, tBarrel, 565, 400),
													gameElement(pBarrel, tBarrel, 760, 230),
													gameElement(pBarrel, tBarrel, 1024, 475) };

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
					case SDL_KEYDOWN:
						/* Touche appuyée, changement de statut */
						switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							turretAngle -= turretSpeed;

							if (turretAngle < -205) {
								turretAngle = -205;
							}
							break;
						case SDLK_RIGHT:
							turretAngle += turretSpeed;
							if (turretAngle > 25) {
								turretAngle = 25;
							}
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
						SDL_Rect backgroundDest = {0, 0, pBackground->w, pBackground->h };

						// Dessin du décor
						SDL_RenderCopy(renderer, tBackground, NULL, &backgroundDest);  // Dessin du fond de l'écran

						// Tourelle du tank
						// TODO
						SDL_Rect turretRect = { 80, 485, pTurret->w, pTurret->h };
						if (SDL_RenderCopyEx(renderer, tTurret, NULL, &turretRect, turretAngle, &turretCenter, SDL_FLIP_NONE) != 0) {
							fprintf(stdout, "Erreur affichage tourelle (%s)\n", SDL_GetError());
						}

						// Dessin du tank et des barils
						// Corps du tank + Barils
						for (int i = 0; i < MAX_ELEMENTS; i++) {
							SDL_RenderCopy(renderer, gameElements[i].texture, NULL, &gameElements[i].eltRect); // Dessin de chaque élément
						}

						// Dessin de l'obus
						// TODO

						SDL_RenderPresent(renderer); // Affichage
					}
					else
					{
						fprintf(stdout, "Échec de chargement du sprite (%s)\n", SDL_GetError());
					}

				}

			}

		}

		SDL_DestroyTexture(tBackground);
		SDL_DestroyTexture(tBarrel);
		SDL_DestroyTexture(tTank);
		SDL_DestroyTexture(tTurret);
		SDL_DestroyTexture(tBullet);

	}

	// Libération de la mémoire pour les Sprites
	SDL_FreeSurface(pBackground);
	SDL_FreeSurface(pBarrel);
	SDL_FreeSurface(pTank);
	SDL_FreeSurface(pTurret);
	SDL_FreeSurface(pBullet);

	//Destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}