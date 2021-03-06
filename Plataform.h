#ifndef PLATAFORM_H
#define PLATAFORM_H

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "./File.h"
#include "./utils.h"
#include "TextureManager.h"
#include "gamesound.h"

#define images_path "./images/"

#include <iostream>
using namespace std;

//hijita  
GLint floor_texture;
GLint iron_texture;
GLint coin_texture;
GLint pipe_texture;
GLint pipe_body_texture;
GLint villain_texture;
GLint sky_texture;





/*
    We asume we have a 800x600 window
    Then we divide this window in 16x12 regions
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    | 12 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    | 11 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    | 10 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |  9 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |  8 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |  7 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |  6 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |  5 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |  4 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |  3 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |  2 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |  1 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    Each column is a line in the file
    Thus, each line has 12 strings, separated by a space
    Those strings have the following meaning:
        a: air
        f: floor
        i: iron
        p: pipe
        c: coin
        b: black square
        v: villain
    Each line is put in the corresponding column from bottom to top
*/


class Plataform {
    public:
        explicit Plataform(const string& filename) {
            File file(filename);
            string line;
            while (file.read_line(line)) {
                vector<string> col;
                split(line, ' ', col);
                objects.push_back(col);
            }
        }
        void draw_sky(double x) {
            glBindTexture(GL_TEXTURE_2D, sky_texture);
                                                                                    
            //glColor3f(0, 0, 0);
            glBegin(GL_QUADS);
         
                glTexCoord2f(0.0f, 0.0f);
                glVertex3d(x-150, 0, -10);
                
                glTexCoord2f(0.0f, 0.5f);
                glVertex3d(x-150, 600, -10);
                
                glTexCoord2f(0.5f, 0.5f);
                glVertex3d(x+800-150, 600, -10);
                
                glTexCoord2f(0.5f, 0.0f);
                glVertex3d(x+800-150, 0, -10);
                
            glEnd();
        }
        void draw(double x) {   // x is the pos of the hero
            glLoadIdentity();
            draw_sky(x);
            for (int i = 0; i < objects.size(); ++i) {
                for (int j = 0; j < objects[i].size(); ++j) {
                    if (objects[i][j] == "f") {
                        // floor
                        // draw a block
                        
                        //funcion de transparencia
                        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        //utilizar transparencia
                        //glEnable(GL_BLEND);
                        glBindTexture(GL_TEXTURE_2D, floor_texture);
                                                                                                
                        //glColor3f(0, 0, 0);
                        glBegin(GL_QUADS);
                     
                           	glTexCoord2f(0.0f, 0.0f);
                            glVertex3d(50*i, 50*j, 0);
                            
                            glTexCoord2f(0.0f, 0.5f);
                            glVertex3d(50*i, 50*(j+1), 0);
                            
                            glTexCoord2f(0.5f, 0.5f);
                            glVertex3d(50*(i+1), 50*(j+1), 0);
                            
                            glTexCoord2f(0.5f, 0.0f);
                            glVertex3d(50*(i+1), 50*j, 0);
                            
                        glEnd();
                        //glDisable(GL_BLEND);    
                    } else if(objects[i][j] == "i") {
                        // wall
                        // not implemented yet
                        glBindTexture(GL_TEXTURE_2D, iron_texture);
                                                                                                
                        //glColor3f(0, 0, 0);
                        glBegin(GL_QUADS);
                     
                            glTexCoord2f(0, 0);
                            glVertex3d(50*i, 50*j, 0);
                            
                            glTexCoord2f(0, 1);
                            glVertex3d(50*i, 50*(j+1), 0);
                            
                            glTexCoord2f(1, 1);
                            glVertex3d(50*(i+1), 50*(j+1), 0);
                            
                            glTexCoord2f(1, 0);
                            glVertex3d(50*(i+1), 50*j, 0);
                            
                        glEnd();
                        
                    } else if(objects[i][j] == "p") {
                        // pipe

                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
                        glEnable(GL_BLEND);//utilizar transparencia   
                        glBindTexture(GL_TEXTURE_2D, pipe_texture);
                                                                                                
                        //glColor3f(0, 0, 0);
                        glBegin(GL_QUADS);
                     
                            glTexCoord2f(0, 0);
                            glVertex3d(50*i, 50*j, 5);
                            
                            glTexCoord2f(0, 1);
                            glVertex3d(50*i, 50*(j+1), 5);
                            
                            glTexCoord2f(1, 1);
                            glVertex3d(50*(i+1), 50*(j+1), 5);
                            
                            glTexCoord2f(1, 0);
                            glVertex3d(50*(i+1), 50*j, 5);
                            
                        glEnd();
                        glDisable(GL_BLEND);
                        
                    } else if(objects[i][j] == "q") {
                        // pipe body

                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
                        glEnable(GL_BLEND);//utilizar transparencia   
                        glBindTexture(GL_TEXTURE_2D, pipe_body_texture);
                                                                                                
                        //glColor3f(0, 0, 0);
                        glBegin(GL_QUADS);
                     
                            glTexCoord2f(0, 0);
                            glVertex3d(50*i, 50*j, 5);
                            
                            glTexCoord2f(0, 1);
                            glVertex3d(50*i, 50*(j+1), 5);
                            
                            glTexCoord2f(1, 1);
                            glVertex3d(50*(i+1), 50*(j+1), 5);
                            
                            glTexCoord2f(1, 0);
                            glVertex3d(50*(i+1), 50*j, 5);
                            
                        glEnd();
                        glDisable(GL_BLEND);
                        
                    } else if(objects[i][j] == "c") {
                        // coin
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
                        glEnable(GL_BLEND);//utilizar transparencia   
                        glBindTexture(GL_TEXTURE_2D, coin_texture);
                                                                                                
                        //glColor3f(0, 0, 0);
                        glBegin(GL_QUADS);
                     
                            glTexCoord2f(0, 0);
                            glVertex3d(50*i, 50*j, 5);
                            
                            glTexCoord2f(0, 1);
                            glVertex3d(50*i, 50*(j+1), 5);
                            
                            glTexCoord2f(1, 1);
                            glVertex3d(50*(i+1), 50*(j+1), 5);
                            
                            glTexCoord2f(1, 0);
                            glVertex3d(50*(i+1), 50*j, 5);
                            
                        glEnd();
                        glDisable(GL_BLEND);
                    } else if(objects[i][j] == "b") {                                               
                        glColor3f(0, 0, 0);
                        glBegin(GL_QUADS);
                            glVertex3d(50*i, 50*j, 6);
                            glVertex3d(50*i, 50*(j+1), 6);
                            glVertex3d(50*(i+1), 50*(j+1), 6);
                            glVertex3d(50*(i+1), 50*j, 6);                            
                        glEnd();
                    } else if(objects[i][j] == "v") {
                        // coin
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
                        glEnable(GL_BLEND);//utilizar transparencia   
                        glBindTexture(GL_TEXTURE_2D, villain_texture);
                                                                                                
                        //glColor3f(0, 0, 0);
                        glBegin(GL_QUADS);
                     
                            glTexCoord2f(0, 0);
                            glVertex3d(50*i, 50*j, 5);
                            
                            glTexCoord2f(0, 1);
                            glVertex3d(50*i, 50*(j+1), 5);
                            
                            glTexCoord2f(1, 1);
                            glVertex3d(50*(i+1), 50*(j+1), 5);
                            
                            glTexCoord2f(1, 0);
                            glVertex3d(50*(i+1), 50*j, 5);
                            
                        glEnd();
                        glDisable(GL_BLEND);
                    }
                }
            }
        }
        string object_in(double x, double y) {
            int i = (int)x / 50;
            int j = (int)y / 50;
            if (i >= objects.size() || j >= 12){
                return "a";
            }
            return objects[i][j];
        }
        void remove_object_in(double x, double y) {
            int i = (int)x / 50;
            int j = (int)y / 50;
            if (i >= objects.size() || j >= 12){
                return;
            }
            objects[i][j] = "a";
        }
        bool hit_object_in(double x, double y) {
            // hit object in (x,y) and return if object was removed
            if (!is_hittable(object_in(x,y))) {
                return false;
            } else {
                remove_object_in(x, y);
                return true;
            }
        }
        bool collect_object_in(double x, double y) {
            // collect object in (x,y) and return if object was removed
            if (!is_collectable(object_in(x,y))) {
                return false;
            } else {
                remove_object_in(x, y);
                return true;
            }
        }
        bool is_solid(const string& object) {
            return object == "f" || object == "i" || object == "p" || 
                   object == "q" || object == "v";
        }

        bool is_hittable(const string& object) {
            return object == "v";
        }

        bool is_collectable(const string& object) {
            return object == "c";
        }
    private:
        vector<vector<string> > objects;      // this matrix represent plataform

};

#endif
