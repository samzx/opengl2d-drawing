//
//  render.cpp
//  lab1
//
//  Created by Sam Xie on 1/2/18.
//  Copyright © 2018 University of Singapore. All rights reserved.
//

#include "render.hpp"

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
    int fps = ONE_SECOND/((glutGet(GLUT_ELAPSED_TIME) - last_time));
    // Clear screen
    for(int i=0; i<50; i++) std::cout<<"\n";
    // Print FPS
    std::cout<<"FPS: "<< fps << " ";
    for(int i=0; i<fps; i++) std::cout<<"|";
    std::cout<<"\n";
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

class Shape {
public:
    GLenum drawMethod;
    Vector4f color;
public:
    void start_drawing(GLenum, Vector4f);
};
void Shape::start_drawing(GLenum draw_method, Vector4f color = Vector4f(0,0,0,1)){
    // Helper method for polygons
    glBegin(draw_method);
    glColor4f(color.w, color.x, color.y, color.z);
}

// Represents a Triangle with three verticies.
class Triangle : Shape {
public:
    Vector2f
        v1 = Vector2f(),
        v2 = Vector2f(),
        v3 = Vector2f();
        GLenum drawMethod;
        Vector4f color;
public:
    void draw();
    void drawMultiColor(Vector4f, Vector4f, Vector4f);
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
    start_drawing(drawMethod, color);
        glVertex2f(v1.x, v1.y);
        glVertex2f(v2.x, v2.y);
        glVertex2f(v3.x, v3.y);
    glEnd();
}
void Triangle::drawMultiColor(Vector4f c1, Vector4f c2, Vector4f c3){
    start_drawing(drawMethod);
    glColor4f(c1.w, c1.x, c1.y, c1.z);
    glVertex2f(v1.x, v1.y);
    glColor4f(c2.w, c2.x, c2.y, c2.z);
    glVertex2f(v2.x, v2.y);
    glColor4f(c3.w, c3.x, c3.y, c3.z);
    glVertex2f(v3.x, v3.y);
    glEnd();
}

// Arc given center, radius and degrees it spans
class Arc : Shape{
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

class Particle : public Arc{
private:
    Vector2f velocity = Vector2f();
public:
    void set_velocity(Vector2f velocity);
    void traverse(float, float);
};
void Particle::set_velocity(Vector2f velocity){
    this->velocity = velocity;
}
void Particle::traverse(float delta_time, float drag = 0){
    float deltaX = velocity.x * delta_time;
    float deltaY = velocity.y * delta_time;
    center = Vector2f(center.x + deltaX, center.y + deltaY);
    // Drag
    float multiplier = 1 - drag * delta_time;
    velocity = Vector2f(velocity.x * multiplier, velocity.y * multiplier);
}

///////////////////////////////////////////////////////////
// Begin psedu-class (render)

#define NUM_PARTICLES 200
#define PARTICLE_VICINITY 2
#define PARTICLE_SPEED 1

Particle particles[NUM_PARTICLES];
FrameManager* frame_manager = new FrameManager();

void init_particles(){
    for(int i=0; i<NUM_PARTICLES; i++){
        const float ANGLE_PRECISION = 36000.0f;
        const float VELOCITY_PRECISION = 100.0f;
        const float ALPHA_PRECISION = 100.0f;
        
        const float PARTICLE_BOUNDS = 15; // 15 | 1 explosion
        const float MAX_VELOCITY = 50.0f; // 10 | 50 explosion
        const float ALPHA_MIN = 0.25f;
        
        // Random position
        float angle = (rand()%(int)ANGLE_PRECISION) / ANGLE_PRECISION * 2 * M_PI;
        float r1 = rand()%(int)PARTICLE_BOUNDS * cos(angle);
        float r2 = rand()%(int)PARTICLE_BOUNDS * sin(angle);
        // Random velocities
        angle = (rand()%(int)ANGLE_PRECISION) / ANGLE_PRECISION * 2 * M_PI;
        float r3 = rand()%(int)VELOCITY_PRECISION / VELOCITY_PRECISION * MAX_VELOCITY * cos(angle);
        float r4 = rand()%(int)VELOCITY_PRECISION / VELOCITY_PRECISION * MAX_VELOCITY * sin(angle);
        
        // Random alpha
        float r5 = (rand() % (int)ALPHA_PRECISION)/ALPHA_PRECISION + ALPHA_MIN;
        r5 = r5 > 1 ? 1 : r5;
        
        // Set states
        particles[i].set(Vector2f(r1, r2), 0.1f * r5, 0, 360, GL_POLYGON, Vector4f(1,1,1,r5));
        particles[i].set_velocity(Vector2f(PARTICLE_SPEED * r3 * r5, PARTICLE_SPEED * r4));
    }
}

// Initilisations before render loop
void startRender(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glLoadIdentity();
    glTranslatef(0, -2, 0);
    
    std::cout<<"YES";
    
    init_particles();
}

// Tidies up and updates states after one render loop
void finish_render(){
    frame_manager->print_fps();
    frame_manager->update();
    
    // Re-render to start next frame
    glutPostRedisplay();
}

void draw_particles(){
    for(int i=0; i<NUM_PARTICLES; i++){
        particles[i].draw();
        particles[i].traverse(frame_manager->delta_time(), 5.0f);
    }
}

void draw_link(int i, int j){
    const float OPACITY_SCALE = 0.25f;
    float x1 = particles[i].center.x;
    float x2 = particles[j].center.x;
    float y1 = particles[i].center.y;
    float y2 = particles[j].center.y;
    float deltaX = x1 - x2;
    float deltaY = y1 - y2;
    float vicinitySq = PARTICLE_VICINITY * PARTICLE_VICINITY;
    float deltaSumSq = deltaX * deltaX + deltaY * deltaY;
    bool isNear = deltaSumSq < vicinitySq;
    if(isNear){
        glBegin(GL_LINES);
            glColor4f(1, 1, 1, (1 - deltaSumSq/vicinitySq) * OPACITY_SCALE * particles[i].color.z);
            glVertex2f(x1, y1);
            glColor4f(1, 1, 1, (1 - deltaSumSq/vicinitySq) * OPACITY_SCALE * particles[j].color.z);
            glVertex2f(x2, y2);
        glEnd();
    }
}

void draw_links(){
    // Can improve if using quadrants
    for(int i=0; i<NUM_PARTICLES; i++){
        for(int j=0; j<NUM_PARTICLES; j++){
            draw_link(i, j);
        }
    }
}

void draw_background(){
    Triangle* tri = new Triangle();
    float scale = 10;//30;
    Vector2f center = Vector2f(0,0);
    tri->set(Vector2f(-0.866 * scale + center.x, -0.5 * scale + center.y),
             Vector2f(0 + center.x, 1 * scale+ center.y),
             Vector2f(0.866 * scale + center.x, -0.5 * scale+ center.y),
             GL_POLYGON,
             Vector4f());
    tri->drawMultiColor(Vector4f(0, 0, 0.0f, 1),
                        Vector4f(0, 0, 0.2f, 1),
                        Vector4f(0.2f, 0, 0.2f, 1));
    
    //TEMP: use function
    scale = 7.1;
    tri->set(Vector2f(-0.866 * scale + center.x, -0.5 * scale + center.y),
             Vector2f(0 + center.x, 1 * scale+ center.y),
             Vector2f(0.866 * scale + center.x, -0.5 * scale+ center.y),
             GL_POLYGON,
             Vector4f());
    tri->drawMultiColor(Vector4f(1, 1, 1, 1),
                        Vector4f(1, 1, 1, 1),
                        Vector4f(1, 1, 1, 1));
    // TEMP: use function
    scale = 7;
    tri->set(Vector2f(-0.866 * scale + center.x, -0.5 * scale + center.y),
             Vector2f(0 + center.x, 1 * scale+ center.y),
             Vector2f(0.866 * scale + center.x, -0.5 * scale+ center.y),
             GL_POLYGON,
             Vector4f());

    tri->drawMultiColor(Vector4f(0.2f, 0, 0.2f, 1),
                        Vector4f(0, 0, 0.0f, 1),
                        Vector4f(0, 0, 0.2f, 1));
    delete tri;
}

void draw_mountains(){
    
}
void draw_forest(){
    
}

void draw_foreground(){
    draw_mountains();
    draw_forest();
}
void draw_moon(){
    const float MOON_SIZE = 0.5f;
    const Vector2f MOON_POSITION = Vector2f(0,0);
    
    // Main moon
    Arc *arc = new Arc();
    arc->set(MOON_POSITION, MOON_SIZE, 0, 360, GL_POLYGON ,Vector4f(1,1,1,1));
    arc->draw();
    delete arc;
    
    // Moon lighting
    int NUM_ARCS = 10;
    float DIST_APART = 0.25f;
    Arc arcs[NUM_ARCS];
    for(int i=0; i<NUM_ARCS; i++){
        float radius = (float)i * DIST_APART;
        float alpha = (1-(float)(i-1)/NUM_ARCS);
        arcs[i].set(MOON_POSITION, MOON_SIZE + radius*radius, 0, 360, GL_POLYGON, Vector4f(1, 1, 1, alpha * alpha * 0.5f));
        arcs[i].draw();
    }
}

void main_render(){
    draw_background();
    draw_links();
    draw_particles();
    draw_foreground();
//    draw_moon();
}

void render(){
    main_render();
    finish_render();
}
