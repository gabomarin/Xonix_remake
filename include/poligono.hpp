#ifndef POLIGONO_H
#define POLIGONO_H


#include <vector>
#include <GL/gl.h>
#include <boost/concept_check.hpp>

#include "xonix.hpp"



struct vertice
{
    GLuint x,y;
};

class Poligono
{

public:
    Poligono();
    virtual ~Poligono();
    void draw();
    void insertVertex( GLuint x, GLuint y, int pos);
    int getVertexX(int vertex);
    int getVertexY(int vertex);
    int getW();
    int getH();
    
private:
    int nVertices;
    struct vertice v[4];
    GLuint textura_;
    string file_;
    SDL_Surface *bmp;
    int width_,height_;
    
    
};

#endif // POLIGONO_H
