#include <boost/concept_check.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <vector>

#include "include/timer.hpp"
#include "include/xonix.hpp"
#include "include/personaje.hpp"
#include "include/enemigo.hpp"
#include "include/poligono.hpp"

void mainLoop();
void actualizaEnemigos(Enemigo enemigo[], int nEnemigos);

void inicializaBordes(vector <Poligono> &poligonos);
void colisiones(vector< Poligono >& poligonos, Personaje& heroe, Enemigo enemigos[], int nEnemigos);

bool recorta(vector <Poligono> &poligonos, Personaje &heroe);
void drawHud(Personaje &heroe, vector <Poligono> &poligonos );


int main( int argc, char *argv[] )
{


    //Inicializa modo grafico
    if( init() == false )
    {
        return EXIT_FAILURE;
    }

    mainLoop();

    //Sale del modo grafico
    clean_up();

    return EXIT_SUCCESS;
}


//Ciclo principal del juego
void mainLoop()
{
    int mouse_x,x, y, mouse_y,i,nPoligonos=0 ;
    double movementX=0, movementY=0;
    //SDL_Event keyevent;

    //The frame rate regulator
    Timer fps;

    Personaje heroe;
    Enemigo enemigo[4];
    vector <Poligono> poligonos(4);
    inicializaBordes(poligonos);

    nPoligonos=4;

    for(int i=0; i<4; i++)
        enemigo[0].setEstado(true);
    int nEnemigos=2;
    //enemigo.push_back();
    char aux[10];
    bool quit = false;
    TTF_Font* tmpfont;
    tmpfont = TTF_OpenFont("resources/impact.ttf", 36);
    if (tmpfont == NULL)
    {

        printf("Unable to load font: %s \n", TTF_GetError());
        exit(-1);
    }
    while( quit == false && heroe.getVidas()>0)
    {
        //Start the frame timer

        fps.start();
        //While there are events to handle

        while( SDL_PollEvent( &event ) )
        {

            switch(event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:

                if(heroe.getEstado()==NO_RECORTANDO)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        quit=true;
                        break;

                    case SDLK_UP:

                        /*movementY=-3*/
                        ;
                        if(heroe.getDireccion()==DOWN)
                            heroe.setDireccion(NINGUNA);
                        else
                            heroe.setDireccion(UP);
                        break;

                    case SDLK_DOWN:
//                         movementY=3;
                        if(heroe.getDireccion()==UP)
                            heroe.setDireccion(NINGUNA);
                        else
                            heroe.setDireccion(DOWN);
                        break;

                    case SDLK_LEFT:
                        if(heroe.getDireccion()==RIGHT)
                            heroe.setDireccion(NINGUNA);
                        else
                            heroe.setDireccion(LEFT);
                        break;


                    case SDLK_RIGHT:
                        if(heroe.getDireccion()==LEFT)
                            heroe.setDireccion(NINGUNA);
                        else
                            heroe.setDireccion(RIGHT);
                        break;
                    }

                }

                else if(heroe.getEstado()==RECORTANDO)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        quit=true;
                        break;

                    case SDLK_UP:

                        /*movementY=-3*/

                        if(heroe.getDireccion()==DOWN)
                            heroe.setDireccion(DOWN);
                        else
                            heroe.setDireccion(UP);
                        break;

                    case SDLK_DOWN:
//                         movementY=3;
                        if(heroe.getDireccion()==UP)
                            heroe.setDireccion(UP);
                        else
                            heroe.setDireccion(DOWN);
                        break;

                    case SDLK_LEFT:
                        if(heroe.getDireccion()==RIGHT)
                            heroe.setDireccion(RIGHT);
                        else
                            heroe.setDireccion(LEFT);
                        break;


                    case SDLK_RIGHT:
                        if(heroe.getDireccion()==LEFT)
                            heroe.setDireccion(LEFT);
                        else
                            heroe.setDireccion(RIGHT);
                        break;
                    }
                }

                break;


            }
        }

        switch(heroe.getDireccion())
        {

        case UP:
            movementY=-3;
            movementX=0;
            break;

        case DOWN:
            movementY=3;
            movementX=0;
            break;

        case LEFT:
            movementY=0;
            movementX=-3;
            break;

        case RIGHT:
            movementY=0;
            movementX=3;
            break;

        case NINGUNA:
            movementY=0;
            movementX=0;
            break;
        }

        glClear( GL_COLOR_BUFFER_BIT );
        //Actualiza las posiciones de los personajes
        heroe.setX(heroe.getX()+movementX);
        heroe.setY(heroe.getY()+movementY);
        actualizaEnemigos(enemigo, nEnemigos);
        colisiones(poligonos,heroe, enemigo,nEnemigos);
        if(recorta(poligonos,heroe))
            heroe.setEstado(NO_RECORTANDO);
        else
            heroe.setEstado(RECORTANDO);


        //Dibuja los poligonos dibujados
        for(i=0; i<poligonos.size(); i++)
            poligonos.at(i).draw();

        //Dibuja en pantalla
        heroe.draw();
	//drawHud(heroe,poligonos);


        SDL_GL_SwapBuffers();

        //FPS del juego
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / (FRAMES_PER_SECOND) ) - fps.get_ticks() );

        }
    }
}



void actualizaEnemigos(Enemigo enemigo[], int nEnemigos)
{
    for(int i=0; i<nEnemigos; ++i)
    {
	enemigo[i].setPosicion();
        enemigo[i].draw();
        
    }
}


void inicializaBordes(vector <Poligono> &poligonos)
{
    //coordenadas del borde izquierdo
    poligonos.at(0).insertVertex(0,0,TOP_LEFT);
    poligonos.at(0).insertVertex(34,0,TOP_RIGHT);
    poligonos.at(0).insertVertex(0,GAME_HEIGHT,BOTTOM_LEFT);
    poligonos.at(0).insertVertex(34,GAME_HEIGHT,BOTTOM_RIGHT);

    //coordenadas del borde superiosprintf(aux,"%i FPS", fps.get_ticks());

    poligonos.at(1).insertVertex(0,0,TOP_LEFT);
    poligonos.at(1).insertVertex(SCREEN_WIDTH, 0, TOP_RIGHT);
    poligonos.at(1).insertVertex(0, 34, BOTTOM_LEFT);
    poligonos.at(1).insertVertex(SCREEN_WIDTH, 34, BOTTOM_RIGHT);

    //coordenadas del borde derecho
    poligonos.at(2).insertVertex(SCREEN_WIDTH-34,0,TOP_LEFT);
    poligonos.at(2).insertVertex(SCREEN_WIDTH,0,TOP_RIGHT);
    poligonos.at(2).insertVertex(SCREEN_WIDTH-34,GAME_HEIGHT,BOTTOM_LEFT);
    poligonos.at(2).insertVertex(SCREEN_WIDTH,GAME_HEIGHT,BOTTOM_RIGHT);

    //coordenadas del borde inferior
    poligonos.at(3).insertVertex(0,GAME_HEIGHT-34,TOP_LEFT);
    poligonos.at(3).insertVertex(SCREEN_WIDTH, GAME_HEIGHT-34, TOP_RIGHT);
    poligonos.at(3).insertVertex(0, GAME_HEIGHT, BOTTOM_LEFT);
    poligonos.at(3).insertVertex(SCREEN_WIDTH, GAME_HEIGHT, BOTTOM_RIGHT);


}


void colisiones(vector <Poligono> &poligonos, Personaje &heroe, Enemigo enemigos[], int nEnemigos)
{
    bool bandera=false;
    int i,j;
    int w1, h1, w2, h2, x1, x2, y1, y2;
    //detecta colision de enemigo con heroe, por completar
    for( i=0; i<nEnemigos; i++)
    {

        w1 = heroe.getW();
        h1 = heroe.getH();
        x1 = heroe.getX();
        y1 = heroe.getY();

        w2 = enemigos[i].getW();
        h2 = enemigos[i].getH();
        x2 = enemigos[i].getX();
        y2 = enemigos[i].getY();

        if(((x1 + w1-4) > x2) && ((y1 + h1-4) > y2) && ((x2 + w2) > x1+4) && ((y2 + h2) > y1+4))
        {
            // printf("COLISION");
            heroe.setVidas(heroe.getVidas()-1);
            printf("\nVidas: %i", heroe.getVidas());
            SDL_Delay(200);
            heroe.inicializa();
        }
// 	else
// 	    printf("\nNO Colision");
//exit(1);
    }



    //Colision con poligonos (en pruebas aun)
    for(i=0; i<nEnemigos; i++)
    {

        w2 = enemigos[i].getW();
        h2 = enemigos[i].getH();
        x2 = enemigos[i].getX();
        y2 = enemigos[i].getY();

        for(j=0; j< poligonos.size(); j++)
        {
            w1 = poligonos.at(j).getVertexX(TOP_RIGHT);
            h1 = poligonos.at(j).getVertexY(BOTTOM_RIGHT);
            x1 = poligonos.at(j).getVertexX(TOP_LEFT);
            y1 = poligonos.at(j).getVertexY(TOP_LEFT);

            //cara derecha del poligono
            if( ((w1) > x2) && ((y1 + h1-1) > y2) && ((x2 + w2) > w1) && ((y2 + h2-1) > y1) )
            {
                if(enemigos[i].getDirection()==BOTTOM_LEFT)
                    enemigos[i].setDirection(BOTTOM_RIGHT);
                else if(enemigos[i].getDirection()==TOP_LEFT)
                    enemigos[i].setDirection(TOP_RIGHT);
                //printf("\nBAZINGA");
                //SDL_Delay(100);
            }

            //cara izquierda del poligono((x1 + w1-4) > x2) && ((y1 + h1-4) > y2) && ((x2 + w2) > x1+4) && ((y2 + h2) > y1+4)
            else if( ((x1) > x2) && ((y1 + h1) > y2) && ((x2 + w2) > x1) && ((y2 + h2) > y1) )
            {
                if(enemigos[i].getDirection()==BOTTOM_RIGHT)
                    enemigos[i].setDirection(BOTTOM_LEFT);
                else if(enemigos[i].getDirection()==TOP_RIGHT)
                    enemigos[i].setDirection(TOP_LEFT);
                //printf("\nBAZINGA");
                //SDL_Delay(100);
            }

            //cara superior del poligono
            else if( ((x1 + w1) > x2) && ((y1) > y2) && ((x2 + w2) > x1) && ((y2 + h2) > y1) )
            {
                if(enemigos[i].getDirection()==BOTTOM_LEFT)
                    enemigos[i].setDirection(TOP_LEFT);
                else if(enemigos[i].getDirection()==BOTTOM_RIGHT)
                    enemigos[i].setDirection(TOP_RIGHT);
                //printf("\nBAZINGA");
                //SDL_Delay(100);
            }


            //cara inferior del poligono
            else if( ((x1 + w1-1) > x2) && ((h1) > y2) && ((x2 + w2) > x1) && ((y2 + h2) > h1) )
            {
                if(enemigos[i].getDirection()==TOP_LEFT)
                    enemigos[i].setDirection(BOTTOM_LEFT);
                else if(enemigos[i].getDirection()==TOP_RIGHT)
                    enemigos[i].setDirection(BOTTOM_RIGHT);
                //printf("\nBAZINGA");
                //SDL_Delay(100);
            }

            /*******Codigo de colision en prueba


            if((enemigos[i].getX()==poligonos.at(j).getVertexX(TOP_LEFT)) && (enemigos[i].getY()==poligonos.at(j).getVertexY(TOP_LEFT)))
            {
                if(enemigos[i].getDirection()==TOP_LEFT)
                enemigos[i].setDirection(TOP_RIGHT);
                if(enemigos[i].getDirection()==BOTTOM_LEFT)
                enemigos[i].setDirection(BOTTOM_RIGHT);

            }********/

        }
    }


}


bool recorta(vector <Poligono> &poligonos, Personaje &heroe)
{
    bool bandera;
    int i,j;
    int w1, h1, w2, h2, x1, x2, y1, y2;
    w2 = heroe.getW();
    h2 = heroe.getH();
    x2 = heroe.getX();
    y2 = heroe.getY();
    for(j=0; j< poligonos.size(); j++)
    {
        w1 = poligonos.at(j).getVertexX(TOP_RIGHT);
        h1 = poligonos.at(j).getVertexY(BOTTOM_RIGHT);
        x1 = poligonos.at(j).getVertexX(TOP_LEFT);
        y1 = poligonos.at(j).getVertexY(TOP_LEFT);

        //toca alguna de las caras del poligono
        //if( ((x1 + w1-32) > x2) && ((y1 + h1-32) > y2) && ((x2 + w2-32) > x1) && ((y2 + h2-32) > y1))
        if((x2<x1+w1) &&(x1 <x2+w2) && (y2<y1+h1) && (y1 < y2+h2))
        {
            bandera=true;
            return bandera;
            //printf("\nBAZINGA");
            //SDL_Delay(100);
        }


        else
            bandera=false;
    }
    return bandera;
}


void drawHud(Personaje &heroe, vector <Poligono> &poligonos )
{
    SDL_Surface *texto;
    SDL_Rect coord;
    SDL_Color color= {255,255,255};
    GLuint textura;
    string text;
    int h,w;
    //imprime vidas
    text="Vidas";
    text.append(convertInt(heroe.getVidas()));
    //texto= TTF_RenderText_Solid(&tmpfont,text.c_str(), Color);
    coord.x=SCREEN_WIDTH/2;
    coord.y=SCREEN_HEIGHT/2;
    //textura= textToTex( text, textura, tmpfont);
    //glBindTexture( GL_TEXTURE_2D, textura );
//     SDL_GL_RenderText(text, color, &coord);
    SDL_GL_RenderText(text, color, textura);
    
    

    
}