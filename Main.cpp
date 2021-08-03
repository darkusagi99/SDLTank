//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>

#define PI 3.14159265

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Elements constants
// Tank position
const int TANK_POS_X = 50;
const int TANK_POS_Y = 600;
const SDL_Point turretCenter = { 25, 25 };
const SDL_Point bulletCenter = { 15, 30 };

// Global gameconfig
double TURRET_SPEED = 1;
double BULLET_MAX_SPEED = 30;
double BULLET_ACC = 5;

// Global vars
long a, b, delta;
const int MAX_ELEMENTS = 4;
const int EXPLOSION_RADIUS = 50;

// --Turret vars
double turretAngle = 0;

// -- Bullet vars
int bulletExist = 0;
int bulletspeed = 0;
double bulletAngle = 0;
double startBulletAngle = 0;
int bulletX = 0;
int bulletY = 0;

int prevBulletX = 0;
int prevBulletY = 0;
Uint32* backgroundPixels;
Uint32 currentPixel;
Uint32 referencePixel = 3935951338;
Uint8* startColor;
Uint8 rColor = 0;
Uint8 gColor = 0;
Uint8 bColor = 0;
int nbBytesParPixel = 0;

Uint8 emptyR = 153;
Uint8 emptyG = 217;
Uint8 emptyB = 234;

bool field[SCREEN_WIDTH][SCREEN_HEIGHT] = { 0 };

int bulletXMax = 0;
int bulletXMin = 0;
int bulletYMax = 0;
int bulletYMin = 0;
int tmpValue = 0;

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

void bulletExplosion() {
	bulletExist = 0;
	bulletspeed = 10;

}

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

		// Initialisation des collisions
			SDL_LockSurface(pBackground);
			nbBytesParPixel = pBackground->format->BytesPerPixel;

			for (int px = 0; px < SCREEN_WIDTH; px++) {
				for (int py = bulletYMin; py < SCREEN_HEIGHT; py++) {

					// Extraction de chaque pixel
					startColor = (Uint8*)pBackground->pixels + py * pBackground->pitch + px * nbBytesParPixel;
					bColor = *startColor;
					gColor = *(startColor + 1);
					rColor = *(startColor + 2);

					// Récupération de la couleurs des pixels
					if (rColor == emptyR && gColor == emptyG && bColor == emptyB) {
						field[px][py] = false;
					} else {
						field[px][py] = true;
					}
				}
			}

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
							// Tourner tourelle à gauche
							turretAngle -= TURRET_SPEED;
							if (turretAngle < -205) {
								turretAngle = -205;
							}
							break;
						case SDLK_RIGHT:
							// Tourner tourelle à droite
							turretAngle += TURRET_SPEED;
							if (turretAngle > 25) {
								turretAngle = 25;
							}
							break;
						case SDLK_SPACE:
							// Charger cannon
							if (bulletExist == 0) {
								bulletspeed += BULLET_ACC;
							}

							// Si cannon complètement chargé - tirer
							if (bulletExist == 0 && bulletspeed >= BULLET_MAX_SPEED) {
								bulletAngle = turretAngle;
								startBulletAngle = bulletAngle;
								bulletExist = 1;
								bulletX = 90 + (65 * cos(bulletAngle * PI / 180.0));
								bulletY = 480 + (65 * sin(bulletAngle * PI / 180.0));
							}

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

							// Tirer
							if (bulletExist == 0) {
								bulletAngle = turretAngle;
								startBulletAngle = bulletAngle;
								bulletExist = 1;
								bulletX = 90 + (65 * cos(bulletAngle * PI / 180.0));
								bulletY = 480 + (65 * sin(bulletAngle * PI / 180.0));
							}
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

					// Gestion de la physique de l'obus
					if (bulletExist == 1) {
						bulletXMin = bulletX;
						bulletYMin = bulletY;
						bulletX += bulletspeed * cos(startBulletAngle * PI / 180);
						bulletY = bulletY + (bulletspeed * sin(bulletAngle * PI / 180));

						// Calcul pour les collisions
						bulletXMax = bulletX + pBullet->w * cos(bulletAngle * PI / 180);
						bulletYMax = bulletY + pBullet->h * sin(bulletAngle * PI / 180);

						// Suite MIN - MAX
						if (bulletXMin > bulletXMax) {
							tmpValue = bulletXMax;
							bulletXMax = bulletXMin;
							bulletXMin = tmpValue;

						}


						if (bulletYMin > bulletYMax) {
							tmpValue = bulletYMax;
							bulletYMax = bulletYMin;
							bulletYMin = tmpValue;

						}

						if (bulletAngle >= -90 && bulletAngle != 90) {
							bulletAngle++;
						}
						else if (bulletAngle < -90 && bulletAngle != -270) {
							bulletAngle--;
						}

						// Suppression de l'obus si sortie écran (gauche / droite / bas)
						if (bulletX < -30 || bulletX  > SCREEN_WIDTH  + 30 || bulletY > SCREEN_HEIGHT + 30) {
							bulletExist = 0;
							bulletspeed = 10;
						}
					
					}


					// TODO - Gestion des collisions
					if (bulletExist == 1) {
					
						// Contrôle collision avec éléments
						for (int i = 0; i < MAX_ELEMENTS; i++) {

							// Contrôle sur les X, puis les Y
							if (gameElements[i].active == 1) {
								if (gameElements[i].xmin <= bulletXMax && gameElements[i].xmax >= bulletXMin) {
									if (gameElements[i].ymin <= bulletYMax && gameElements[i].ymax >= bulletYMin) {
										gameElements[i].active = 0;
										bulletExplosion();
									}
								}
							}
						}

						// Contrôle des collisions avec le décor - TODO
						if (bulletXMin > 0 && bulletXMax < SCREEN_WIDTH && bulletYMin > 0 && bulletYMax < SCREEN_HEIGHT) {

							for (int px = bulletXMin; px <= bulletXMax; px++) {
								for (int py = bulletYMin; py <= bulletYMax; py++) {

									if (field[px][py] == true) {
										bulletExplosion();
									}
								}
							}
						}
					}


					//Get window surface
					screenSurface = SDL_GetWindowSurface(window);

					if (pBackground && pBarrel && pTank && pTurret && pBullet)
					{
						SDL_Rect backgroundDest = {0, 0, pBackground->w, pBackground->h };

						// Dessin du décor
						SDL_RenderCopy(renderer, tBackground, NULL, &backgroundDest);  // Dessin du fond de l'écran

						// Tourelle du tank
						SDL_Rect turretRect = { 80, 485, pTurret->w, pTurret->h };
						if (SDL_RenderCopyEx(renderer, tTurret, NULL, &turretRect, turretAngle, &turretCenter, SDL_FLIP_NONE) != 0) {
							fprintf(stdout, "Erreur affichage tourelle (%s)\n", SDL_GetError());
						}

						// Dessin du tank et des barils
						// Corps du tank + Barils
						for (int i = 0; i < MAX_ELEMENTS; i++) {
							if (gameElements[i].active == 1) {
								SDL_RenderCopy(renderer, gameElements[i].texture, NULL, &gameElements[i].eltRect); // Dessin de chaque élément
							}
						}

						// Dessin de l'obus
						if (bulletExist == 1) {
							SDL_Rect bulletRect = { bulletX, bulletY, pBullet->w, pBullet->h };
							SDL_RenderCopyEx(renderer, tBullet, NULL, &bulletRect, bulletAngle + 90, &bulletCenter, SDL_FLIP_NONE); // Dessin de chaque élément
						}


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