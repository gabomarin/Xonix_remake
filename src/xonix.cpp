

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "../include/xonix.hpp"
bool initGL()
{
    glClearColor( .34, .123, .3, 0 );

	glEnable( GL_TEXTURE_2D ); // Need this to display a texture
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        //   printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    return true;
}

bool init()
{
    //Initialize SDL
    
    srand(time(NULL));
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        return false;
    }
    SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL );
    //Create Window
//     if(!screen )
//     {
//         return true;
//     }

    //Enable unicode
    SDL_EnableUNICODE( SDL_TRUE );

    //Initialize OpenGL
    if( initGL() == false )
    {
        return false;
    }
    atexit(SDL_Quit);
    //Initialize text mode
    if (TTF_Init() == -1)
    {
        printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());

        exit(1);
    }
    
    loadFont();

    //Set caption
    SDL_WM_SetCaption( "Xonix", NULL );
    //_putenv("SDL_VIDEO_WINDOW_POS");
    //_putenv("SDL_VIDEO_CENTERED=1");
    return true;
}

void handleKeys( unsigned char key)
{
    //Toggle quad
    if( key == 'q' )
    {
        renderQuad = !renderQuad;
    }
}


void clean_up()
{
    //Quit SDL
    SDL_Quit();
}





//convierte una  imagen de SDL a textura de opengl
GLuint surfaceToTex(string file, GLuint& tex) {

    GLint	nOfColors;
    GLenum	texture_format;
    SDL_Surface *suf = IMG_Load(file.c_str());
    if(!suf) {
        exit(-1);
    }


    // Get the number of channels in the SDL surface
    nOfColors = suf->format->BytesPerPixel;
    if(nOfColors == 4) {
        if(suf->format->Rmask == 0x000000ff) {
            texture_format = GL_RGBA;
        } else {
            texture_format = GL_BGRA;
        }
    } else if(nOfColors == 3) {
        if(suf->format->Rmask == 0x000000ff) {
            texture_format = GL_RGB;
        } else {
            texture_format = GL_BGR;
        }
    } else {
        return 2;
    }

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, tex);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, suf->w, suf->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, suf->pixels);
    /*
    glBindTexture( GL_TEXTURE_2D, tex );
	glBegin(GL_QUADS);
	glVertex3f(10, suf->h+100,0);
	glVertex3f(suf->w+10,100+suf->h,0);
    
	glVertex3f(suf->w+10, 100,0);
	glVertex3f(10,100,0);
	glEnd();*/
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
	return roundi(pow(2,ceil(logbase2)));
}

void SDL_GL_RenderText(string text, SDL_Color color, GLuint& tex)
{
    
    GLint	nOfColors;
    GLenum	texture_format;
    TTF_Font *aux= tmpfont;
    SDL_Surface *suf = TTF_RenderText_Solid(aux,text.c_str(), color);
    if(!suf) {
        exit(-1);
    }


    // Get the number of channels in the SDL surface
    nOfColors = suf->format->BytesPerPixel;
    if(nOfColors == 4) {
        if(suf->format->Rmask == 0x000000ff) {
            texture_format = GL_RGBA;
        } else {
            texture_format = GL_BGRA;
        }
    } else if(nOfColors == 3) {
        if(suf->format->Rmask == 0x000000ff) {
            texture_format = GL_RGB;
        } else {
            texture_format = GL_BGR;
        }
    } else {
        ;
    }

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, tex);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, suf->w, suf->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, suf->pixels);
    
    glBindTexture( GL_TEXTURE_2D, tex );
	glBegin(GL_QUADS);
	glVertex3f(10, suf->h+100,0);
	glVertex3f(suf->w+10,100+suf->h,0);
    
	glVertex3f(suf->w+10, 100,0);
	glVertex3f(10,100,0);
	glEnd();
    SDL_FreeSurface(suf);

}


string convertInt(int number)
{
    unsigned int i;
    if (number == 0)
        return "0";
    string temp="";
    string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for ( i=0; i<temp.length(); i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}

void loadFont()
{
    tmpfont = TTF_OpenFont("resources/impact.ttf", 36);
    if (tmpfont == NULL)
    {

        printf("Unable to load font: %s \n", TTF_GetError());
        exit(-1);
    }
}
