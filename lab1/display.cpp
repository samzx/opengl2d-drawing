//
//  lab1.cpp
//  lab1
//
//  Created by Sam Xie on 30/1/18.
//  Copyright © 2018 University of Singapore. All rights reserved.
//

#include "display.hpp"

// Rotate
float alpha = 0.0;
// Scale
float k=1;
// Transform
float tx = 0.0, ty=0.0;

void init(void){
    glClearColor (1.0, 1.0, 1.0, 1.0);
    glShadeModel (GL_SMOOTH);
}

void reshape (int w, int h){
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(-10, 10, -10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    start_render();
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    
    //controls transformation
    glScalef(k, k, k);
    glTranslatef(tx, ty, 0);
    glRotatef(alpha, 0, 0, 1);
    
    // Render drawings
    render();
    
    glPopMatrix();
    glFlush ();
}

