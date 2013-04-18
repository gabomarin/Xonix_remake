#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <string>



#include "xonix.hpp"

using namespace std;
class Enemigo
{

public:
    Enemigo();
    virtual ~Enemigo();
    void setPosicion();
    void draw();

    void setEstado(bool estado);
    bool getEstado();
    int getX();
    int getY();
    void setDirection(int dir);
    int getDirection();
    int getW();
    int getH();
    
    void setVel(int vel);
    int getVel();

private:
    string file_;
    SDL_Surface *bmp;
    int posx_, posy_;
    int width_, height_;
    int direccion_;
    bool estado_;
    GLuint textura_;
    short int vel_;
};

#endif // ENEMIGO_H
