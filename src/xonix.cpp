#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <unistd.h>

#include "../include/xonix.hpp"

SDL_Event event;
SDL_Rect renderQuad;
TTF_Font* tmpfont;
bool renderQuadEnabled = false;

bool initGL()
{
    glClearColor(0.34, 0.123, 0.3, 0);

    glEnable(GL_TEXTURE_2D); // Need this to display a texture
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        printf("Error initializing OpenGL! %d\n", error);
        return false;
    }

    return true;
}

bool init()
{
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }

    if (!SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL))
    {
        printf("Failed to set video mode: %s\n", SDL_GetError());
        return false;
    }

    SDL_EnableUNICODE(SDL_TRUE);

    if (!initGL())
    {
        return false;
    }

    atexit(SDL_Quit);

    if (TTF_Init() == -1)
    {
        printf("Unable to initialize SDL_ttf: %s\n", TTF_GetError());
        exit(1);
    }

    loadFont();

    SDL_WM_SetCaption("Xonix", NULL);

    return true;
}

void handleKeys(unsigned char key)
{
    if (key == 'q')
    {
        renderQuadEnabled = !renderQuadEnabled;
    }
}

void clean_up()
{
    SDL_Quit();
}

GLuint surfaceToTex(string file, GLuint& tex)
{
    GLint nOfColors;
    GLenum texture_format;
    SDL_Surface* suf = IMG_Load(file.c_str());

    if (!suf)
    {
        printf("Unable to load image %s: %s\n", file.c_str(), IMG_GetError());
        exit(-1);
    }

    nOfColors = suf->format->BytesPerPixel;
    if (nOfColors == 4)
    {
        texture_format = (suf->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
    }
    else if (nOfColors == 3)
    {
        texture_format = (suf->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
    }
    else
    {
        SDL_FreeSurface(suf);
        return 2;
    }

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, suf->w, suf->h, 0, texture_format, GL_UNSIGNED_BYTE, suf->pixels);

    SDL_FreeSurface(suf);
    return tex;
}

int roundi(double x)
{
    return (int)(x + 0.5);
}

int nextpoweroftwo(int x)
{
    double logbase2 = log(x) / log(2);
    return roundi(pow(2, ceil(logbase2)));
}

void SDL_GL_RenderText(string text, SDL_Color color, GLuint& tex)
{
    GLint nOfColors;
    GLenum texture_format;
    SDL_Surface* suf = TTF_RenderText_Solid(tmpfont, text.c_str(), color);

    if (!suf)
    {
        printf("Unable to render text: %s\n", TTF_GetError());
        exit(-1);
    }

    nOfColors = suf->format->BytesPerPixel;
    if (nOfColors == 4)
    {
        texture_format = (suf->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
    }
    else if (nOfColors == 3)
    {
        texture_format = (suf->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
    }
    else
    {
        SDL_FreeSurface(suf);
        return;
    }

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, suf->w, suf->h, 0, texture_format, GL_UNSIGNED_BYTE, suf->pixels);

    SDL_FreeSurface(suf);
}

void loadFont()
{
    tmpfont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 36);
    if (!tmpfont)
    {
        printf("Unable to load font: %s\n", TTF_GetError());
        exit(-1);
    }
}

void updatePlayer(Player& player, const Uint8* keys)
{
    // Movimiento restringido a una sola dirección
    if (keys[SDLK_UP])
    {
        player.y -= player.speed;
    }
    else if (keys[SDLK_DOWN])
    {
        player.y += player.speed;
    }
    else if (keys[SDLK_LEFT])
    {
        player.x -= player.speed;
    }
    else if (keys[SDLK_RIGHT])
    {
        player.x += player.speed;
    }

    // Limitar al área de juego
    if (player.x < 0) player.x = 0;
    if (player.y < 0) player.y = 0;
    if (player.x > SCREEN_WIDTH - 20) player.x = SCREEN_WIDTH - 20;
    if (player.y > SCREEN_HEIGHT - 20) player.y = SCREEN_HEIGHT - 20;
}

void updateEnemies(std::vector<Enemy>& enemies)
{
    for (auto& enemy : enemies)
    {
        enemy.x += enemy.dx;
        enemy.y += enemy.dy;

        // Rebotar en los bordes
        if (enemy.x < 0 || enemy.x > SCREEN_WIDTH - 20) enemy.dx = -enemy.dx;
        if (enemy.y < 0 || enemy.y > SCREEN_HEIGHT - 20) enemy.dy = -enemy.dy;
    }
}

void renderPlayer(const Player& player)
{
    glBindTexture(GL_TEXTURE_2D, player.texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(player.x, player.y, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(player.x + 20, player.y, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(player.x + 20, player.y + 20, 0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(player.x, player.y + 20, 0);
    glEnd();
}

void renderEnemies(const std::vector<Enemy>& enemies)
{
    for (const auto& enemy : enemies)
    {
        glBindTexture(GL_TEXTURE_2D, enemy.texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(enemy.x, enemy.y, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(enemy.x + 20, enemy.y, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(enemy.x + 20, enemy.y + 20, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(enemy.x, enemy.y + 20, 0);
        glEnd();
    }
}

int main(int argc, char* argv[])
{
    chdir("/home/gabo/projects/Xonix");

    if (!init())
    {
        printf("Failed to initialize!\n");
        return -1;
    }

    // Crear jugador
    Player player = {100.0f, 100.0f, 5.0f};
    glGenTextures(1, &player.texture);
    surfaceToTex("resources/heroe.png", player.texture);

    // Crear enemigos
    std::vector<Enemy> enemies(2);
    for (auto& enemy : enemies)
    {
        enemy.x = rand() % SCREEN_WIDTH;
        enemy.y = rand() % SCREEN_HEIGHT;

        // Asegurar que dx y dy no sean 0
        do {
            enemy.dx = (rand() % 3 - 1) * 2.0f; // Valores posibles: -2, 0, 2
        } while (enemy.dx == 0);

        do {
            enemy.dy = (rand() % 3 - 1) * 2.0f; // Valores posibles: -2, 0, 2
        } while (enemy.dy == 0);

        glGenTextures(1, &enemy.texture);
        surfaceToTex("resources/enemigo.png", enemy.texture);
    }

    bool quit = false;
    while (!quit)
    {
        const Uint8* keys = SDL_GetKeyState(NULL);

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Actualizar lógica
        updatePlayer(player, keys);
        updateEnemies(enemies);

        // Renderizar
        glClear(GL_COLOR_BUFFER_BIT);
        renderPlayer(player);
        renderEnemies(enemies);
        SDL_GL_SwapBuffers();
    }

    clean_up();
    return 0;
}
