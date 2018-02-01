//
//  render.cpp
//  lab1
//
//  Created by Sam Xie on 1/2/18.
//  Copyright © 2018 University of Singapore. All rights reserved.
//

#include "render.hpp"

// Stores two floating point numbers relating to x, y positions
class Vector2f{
public:
    float x, y;
public:
    Vector2f(float, float);
    Vector2f();
};
Vector2f::Vector2f(float a, float b) : x(a), y(b) {}
Vector2f::Vector2f() : x(0), y(0) {}

// Represents a Triangle with three verticies.
class Triangle {
public:
    Vector2f
        v1 = Vector2f(),
        v2 = Vector2f(),
        v3 = Vector2f();
public:
    void draw();
    void set(Vector2f, Vector2f, Vector2f);
};
void Triangle::set(Vector2f a, Vector2f b, Vector2f c){
    v1 = a;
    v2 = b;
    v3 = c;
}
void Triangle::draw() {
    glBegin(GL_POLYGON); {
        glVertex2f(v1.x, v1.y);
        glVertex2f(v2.x, v2.y);
        glVertex2f(v3.x, v3.y);
    }
    glEnd();
    glFlush();
}

// Arc given center, radius and degrees it spans
class Arc {
public:
    Vector2f center = Vector2f();
    float radius;
    int startDeg, endDeg;
public:
    void draw();
    void set(Vector2f, float, int, int);
};

void Arc::set(Vector2f c, float r, int sDeg, int eDeg){
    center = c;
    radius = r;
    startDeg = sDeg;
    endDeg = eDeg;
}
void Arc::draw(){
    glBegin(GL_LINE_STRIP); {
        for(int i=startDeg; i<=endDeg; i++){
            glVertex2f(center.x+radius*sin(2.0*M_PI*i/360.0),
                       center.y+radius*cos(2.0*M_PI*i/360.0));
        }
    }
    glEnd();
    glFlush();
}

void render(){
    // Set color to black
    glColor3f(0,0,0);
    
    Arc *arc = new Arc();
    arc->set(Vector2f(), 1, 0, 360);
    arc->draw();
    
    Arc arcs[7];
    for(int i=0; i<7; i++){
        int radius = 8 - i;
        arcs[i].set(Vector2f(), radius, 0, 360);
        arcs[i].draw();
    }
    
    Triangle* tri = new Triangle();
    tri->set(Vector2f(-0.5, -0.5), Vector2f(0, 0.5), Vector2f(0.5, -0.5));
    tri->draw();
    
    int NUM_TRIS = 25;
    srand(time(NULL));
    Triangle tris[NUM_TRIS];
    for(int i=0; i<NUM_TRIS; i++){
        float r1 = (rand() % 1000 - 500.0)/100;
        float r2 = (rand() % 1000 - 500.0)/100;
        std::cout<<r1<<" "<<r2<<"\n";
        tris[i].set(Vector2f(-0.25+r1, -0.25+r2), Vector2f(0+r1, 0.25+r2), Vector2f(0.25+r1, -0.25+r2));
        tris[i].draw();
    }

}
