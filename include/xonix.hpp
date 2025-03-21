#ifndef XONIX_H
#define XONIX_H

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

//Screen attributes
const int SCREEN_WIDTH = 801;
const int SCREEN_HEIGHT = 580;

const int GAME_HEIGHT= SCREEN_HEIGHT-100;
const int SCREEN_BPP = 24;


const int TOP_LEFT=0;
const int BOTTOM_LEFT=1;
const int BOTTOM_RIGHT=2;
const int TOP_RIGHT=3;


const int UP=0;
const int DOWN=1;
const int RIGHT=2;
const int LEFT=3;

#define NINGUNA 4

//The frame rate
const int FRAMES_PER_SECOND = 60;

//Event handler
extern SDL_Event event;

//Rendering flag
extern SDL_Rect renderQuad;
extern bool renderQuadEnabled;
GLuint surfaceToTex(string file, GLuint& tex);

bool initGL();
bool init();
void handleKeys( unsigned char key);
string convertInt(int number);
void clean_up();
void SDL_GL_RenderText(string text, SDL_Color color, GLuint& tex);
int roundi(double x);

int nextpoweroftwo(int x);
void loadFont();

extern TTF_Font* tmpfont;

struct Player {
    float x, y; // Posición del jugador
    float speed; // Velocidad del jugador
    GLuint texture; // Textura del jugador
};

struct Enemy {
    float x, y; // Posición del enemigo
    float dx, dy; // Dirección del movimiento
    GLuint texture; // Textura del enemigo
};

void updatePlayer(Player& player, const Uint8* keys);
void updateEnemies(std::vector<Enemy>& enemies);
void renderPlayer(const Player& player);
void renderEnemies(const std::vector<Enemy>& enemies);

#endif