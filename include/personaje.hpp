#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "xonix.hpp"

//estados del personaje
const int NO_RECORTANDO = 0;
const int RECORTANDO = 1;
const int RECORTADO = 2;

class Personaje
{

public:
    Personaje();
 
    virtual ~Personaje();
    
    void draw();
    
    void setX(double x);
    void setY(double y);
    double getX();
    double getY();
    
    int getVidas();
    void setVidas(int vidas);
    
    int getEstado();
    void setEstado(int estado);
    
    void setDireccion(int direccion);
    int getDireccion();
    int getH();
    int getW();
    void inicializa();

    
    
    
private:
    string file_;
    SDL_Surface *bmp;
    int posx_, posy_;
    int width_, height_;
    int numVidas_;
    int estado_;
    int direccion_;
    GLuint textura_;
    int recorteInicio_, recorteActual_;
    
    
};

#endif // PERSONAJE_H
