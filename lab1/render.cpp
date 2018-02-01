//
//  render.cpp
//  lab1
//
//  Created by Sam Xie on 1/2/18.
//  Copyright © 2018 University of Singapore. All rights reserved.
//

#include "render.hpp"

void drawArc(double cx, double cy, float r,
             int startDeg, int endDeg) {
    glColor3f(0,0,0);
    glBegin(GL_LINE_STRIP); {
        for(int i=startDeg; i<=endDeg; i++){
            glVertex2f(cx+r*sin(2.0*M_PI*i/360.0),
                       cy+r*cos(2.0*M_PI*i/360.0));
        }
    }
    glEnd();
    glFlush();
}

void drawTriangle(){
    glBegin(GL_POLYGON); {
        glVertex2f(-1, -1);
        glVertex2f(0, 1);
        glVertex2f(1, -1);
    }
    glEnd();
    glFlush();
}

void render(){
    drawArc(0,0,8,0,360);
    drawTriangle();
}
