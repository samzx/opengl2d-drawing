//
//  render.cpp
//  lab1
//
//  Created by Sam Xie on 1/2/18.
//  Copyright © 2018 University of Singapore. All rights reserved.
//

#include "render.hpp"
#include <time.h>

// Tracks the update loop
class FrameManager{
private:
    int last_time = 0;
    int ONE_SECOND = 1000;
public:
    void update();
    void print_fps();
    float delta_time();
};
void FrameManager::update(){
    last_time = (glutGet(GLUT_ELAPSED_TIME));
}
void FrameManager::print_fps(){
    std::cout<<"FPS: "<<ONE_SECOND/((glutGet(GLUT_ELAPSED_TIME) - last_time))<<"\n";
}
float FrameManager::delta_time(){
    float delta = ((glutGet(GLUT_ELAPSED_TIME) - last_time));
    return delta/ONE_SECOND;
}

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

// Stores four floating point numbers relating to w, x, y, z positions
class Vector4f{
public:
    float w, x, y, z;
public:
    Vector4f(float, float, float, float);
    Vector4f();
};
Vector4f::Vector4f(float a, float b, float c, float d) : w(a), x(b), y(c), z(d) {}
Vector4f::Vector4f() : w(0), x(0), y(0), z(0) {}

// Helper method for polygons
void start_drawing(GLenum draw_method, Vector4f color = Vector4f(0,0,0,1)){
    glBegin(draw_method);
    glColor4f(color.w, color.x, color.y, color.z);
}

// Represents a Triangle with three verticies.
class Triangle {
public:
    Vector2f
        v1 = Vector2f(),
        v2 = Vector2f(),
        v3 = Vector2f();
    GLenum drawMethod;
    Vector4f color;
public:
    void draw();
    void set(Vector2f, Vector2f, Vector2f, GLenum, Vector4f);
};
void Triangle::set(Vector2f v1, Vector2f v2, Vector2f v3, GLenum drawMethod, Vector4f color = Vector4f(0,0,0,1)){
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    this->color = color;
    this->drawMethod = drawMethod;
}
void Triangle::draw() {
    start_drawing(drawMethod);
        glVertex2f(v1.x, v1.y);
        glVertex2f(v2.x, v2.y);
        glVertex2f(v3.x, v3.y);
    glEnd();
}

// Arc given center, radius and degrees it spans
class Arc {
public:
    Vector2f center = Vector2f();
    float radius;
    int startDeg, endDeg;
    Vector4f color = Vector4f(0,0,0,1);
    GLenum drawMethod;
public:
    void draw();
    void set(Vector2f, float, int, int, GLenum, Vector4f);
};
void Arc::set(Vector2f center, float radius, int startDeg, int endDeg,
              GLenum drawMethod = GL_LINE_STRIP , Vector4f color = Vector4f(0,0,0,1)){
    this->center = center;
    this->radius = radius;
    this->startDeg = startDeg;
    this->endDeg = endDeg;
    this->color = color;
    this->drawMethod = drawMethod;
}

void Arc::draw(){
    start_drawing(drawMethod, color);
    for(int i=startDeg; i<=endDeg; i++){
        glVertex2f(center.x+radius*sin(2.0*M_PI*i/360.0),
                   center.y+radius*cos(2.0*M_PI*i/360.0));
    }
    glEnd();
}

class Particle{
private:
    Arc arc;
    Vector2f *velocity = new Vector2f();
public:
    Particle(Arc);
    void draw();
};
Particle::Particle(Arc a) : arc(a) {}

///////////////////////////////////////////////////////////
// Begin psedu-class (render)

float radius = 0;
FrameManager* frame_manager = new FrameManager();

// Initilisations before render loop
void startRender(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(0,0,0);
}

// Tidies up and updates states after one render loop
void finish_render(){
    frame_manager->print_fps();
    frame_manager->update();
    
    // Swap buffers (show rendered frame)
    glFlush();
    
    // Re-render to start next frame
    glutPostRedisplay();
}

Arc *arc = new Arc();
int NUM_TRIS = 2500;
Triangle tris[2500];

void main_render(){
    radius += frame_manager->delta_time();
    
    arc->set(Vector2f(), 1.0+radius, 0, 360, GL_POLYGON ,Vector4f(1,0,1,0.5f));
    arc->draw();
    
//    int NUM_ARCS = 250;
//    Arc arcs[NUM_ARCS];
//    for(int i=0; i<NUM_ARCS; i++){
//        float radius = 8 - i;
//        arcs[i].set(Vector2f(), radius/NUM_ARCS * 20, 0, 360, GL_LINE_STRIP);
//        arcs[i].draw();
//    }

    Triangle* tri = new Triangle();
    tri->set(Vector2f(-0.5, -0.5), Vector2f(0, 0.5), Vector2f(0.5, -0.5), GL_POLYGON);
    tri->draw();
//
//    glColor4f(0, 0, 0, 1);
//    for(int i=0; i<NUM_TRIS; i++){
//        float r1 = (rand() % 1000 - 500.0)/50;
//        float r2 = (rand() % 1000 - 500.0)/50;
//        tris[i].set(Vector2f(-0.25+r1, -0.25+r2), Vector2f(0+r1, 0.25+r2), Vector2f(0.25+r1, -0.25+r2));
//        tris[i].draw();
//    }
    
}

void render(){
    main_render();
    finish_render();
}
