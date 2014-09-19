#include <cstdio>
#include <cmath>
#include <GL/glut.h>

#include "hero.h"

#define ECHAP 27


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


double plataforma_pos = -50;
double plataforma_pos_delta = -1;

void draw_plataforma() {
    glLoadIdentity();
    glTranslated(plataforma_pos, 0, 0);
    plataforma_pos += plataforma_pos_delta;

    if (plataforma_pos < -950) {
        exit(0);
    }

    glBegin(GL_POLYGON);
        glColor3f(1,0,0);
        glVertex2d(0, -5);
        glVertex2d(0, -40);
        glColor3f(0,1,0);
        glVertex2d(1000, -40);
        glVertex2d(1000, -5);
    glEnd();

    for (int i=100; i<1000; i+=100) {
        glBegin(GL_POLYGON);
            glVertex2d(i, 5);
            glVertex2d(i, -5);
            glVertex2d(i+10, -5);
            glVertex2d(i+10, 5);
        glEnd();        
    }
}

HeroStateManager hero_state_manager;

GLvoid window_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50, 50, -40, 40, -25.0f, 25.0f);
    glMatrixMode(GL_MODELVIEW);

    draw_plataforma();
    hero_state_manager.draw();

    glutSwapBuffers();

    glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

    glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (hero_state_manager.get_state() != HERO_JUMPING){
                hero_state_manager.set_state(HERO_JUMPING);
            }
            break;

        case GLUT_KEY_DOWN:
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