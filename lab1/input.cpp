//
//  input.cpp
//  lab1
//
//  Created by Sam Xie on 1/2/18.
//  Copyright © 2018 University of Singapore. All rights reserved.
//

#include "input.hpp"

using namespace std;

void keyboard (unsigned char key, int x, int y) {
    //keys to control scaling - k
    //keys to control rotation - alpha
    //keys to control translation - tx, ty
    switch (key) {
            
        case 'z':
            alpha+=10;
            glutPostRedisplay();
            break;
            
        case 'c':
            alpha-=10;
            glutPostRedisplay();
            break;
            
        case 'e':
            k+=0.1;
            glutPostRedisplay();
            break;
            
        case 'q':
            if(k>0.1)
                k-=0.1;
            glutPostRedisplay();
            break;
            
        case 'a':
            tx-=0.1;
            glutPostRedisplay();
            break;
            
        case 'd':
            tx+=0.1;
            glutPostRedisplay();
            break;
            
        case 's':
            ty-=0.1;
            glutPostRedisplay();
            break;
            
        case 'w':
            ty+=0.1;
            glutPostRedisplay();
            break;
            
        case 'r':
            display();
//            render();
            break;
        case 27:
            exit(0);
            break;
            
        default:
            break;
    }
}

void printControls(){
    cout<<"+++++CONTROL BUTTONS+++++++\n\n";
    cout<<"Scale Up/Down: E/Q\n";
    cout<<"Rotate Clockwise/Counter-clockwise: C/Z\n";
    cout<<"Move Up/Down: W/S\n";
    cout<<"Move Left/Right: A/D\n";
    cout <<"ESC: Quit\n";
}
