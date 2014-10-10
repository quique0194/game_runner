#include <cstdio>
#include <cmath>
#include <GL/glut.h>

#include <iostream>
using std::cout;
using std::endl;

#include "hero.h"
#include "TextureManager.h"
#include "Plataform.h"

#define ECHAP 27


Plataform plataform("plataform.txt");
HeroStateManager hero_state_manager(&plataform);


GLvoid window_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // follow hero
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double x = hero_state_manager.get_state()->x;
    glOrtho(x-150, x+650, 0, 600, -100, 100);
    
    glMatrixMode(GL_MODELVIEW);

    plataform.draw();
    hero_state_manager.draw();

    glutSwapBuffers();

    glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 600, 0, 800, -100, 100);

    glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            hero_state_manager.get_state()->jump();
            break;

        case GLUT_KEY_DOWN:
            hero_state_manager.get_state()->slide();
            break;

        case GLUT_KEY_LEFT:                 
            break;

        case GLUT_KEY_RIGHT:
            break;

        default:
            printf("La tecla %d no hace nada.\n", key);
            break;
    }
}

//function called on each frame
GLvoid window_idle() {
    glutPostRedisplay();
}

GLvoid initGL() {
    GLfloat position[] = { 0.0f, 5.0f, 10.0f, 0.0 };

    //enable light : try without it
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    //light 0 "on": try without it
    glEnable(GL_LIGHT0);

    //shading model : try GL_FLAT
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);

    //enable material : try without it
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0.3, 0.7, 0);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


    glutInitWindowSize(1000, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Game Runner");


    initGL();

    glutDisplayFunc(&window_display);
    glutReshapeFunc(&window_reshape);
    glutSpecialFunc(&window_key);

    //function called on each frame
    glutIdleFunc(&window_idle);

    glutMainLoop();

    return 1;
}