

#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

using namespace std;

#include "data.h"
#include "functions.h"
#include "callbackFunctions.h"

int main(int argc,  char * argv[]) {
    
    /*Initialize glut stuff*/
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_RGB|GLUT_SINGLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Bunny");
    loadObjFiles("bunny_high.obj", vertice_list, triangles_list);
    initialize();
    
    
    
    /*Initialize gl stuff*/
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.4, 0.4, -0.4 * .48 / .64, 0.4 * .48 / .64, 2, 10); //parallel projection
    gluLookAt( 0, 0, 5, 0, 0.2, 0, 0, 1, 0);
    
    
    /*Register GL stuff with the GLUT*/
    glutDisplayFunc(onDisplay);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMouseMotion);
    glutKeyboardFunc(onKeyboard);
    
    
        
    
    
    
    /*Initialize the loop*/
    glutMainLoop();
    
    
    
    
    return 0;
}
