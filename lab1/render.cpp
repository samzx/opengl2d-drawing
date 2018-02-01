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
};
Vector2f::Vector2f(float a, float b) : x(a), y(b) {}

// Represents a Triangle with three verticies.
class Triangle {
public:
    Vector2f v1, v2, v3;
public:
    Triangle(Vector2f, Vector2f, Vector2f);
    void draw();
};
Triangle::Triangle(Vector2f a, Vector2f b, Vector2f c) : v1(a), v2(b), v3(c) {}
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
    Vector2f center;
    float radius;
    int startDeg, endDeg;
public:
    Arc(Vector2f, float, int, int);
    void draw();
};
Arc::Arc(Vector2f c, float r, int sDeg, int eDeg) : center(c), radius(r), startDeg(sDeg), endDeg(eDeg) {}
void Arc::draw(){
    glColor3f(0,0,0);
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
    
    Arc *arc = new Arc(Vector2f(0,0), 8, 0, 360);
    arc->draw();
    
    
    Triangle* tri = new Triangle(Vector2f(-1, -1), Vector2f(0, 1), Vector2f(1, -1));
    tri->draw();

}
