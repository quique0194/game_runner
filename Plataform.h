#ifndef PLATAFORM_H
#define PLATAFORM_H

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "./File.h"
#include "./utils.h"

#define images_path "./images/"

#include <iostream>
using namespace std;

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
        w: wall
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
        void draw() {
            glLoadIdentity();
            for (int i = 0; i < objects.size(); ++i) {
                for (int j = 0; j < objects[i].size(); ++j) {
                    if (objects[i][j] == "a") {
                        // air
                        // don't draw anything
                    }
                    else if (objects[i][j] == "f") {
                        // floor
                        // draw a block
                        glColor3f(0, 0, 0);
                        glBegin(GL_QUADS);
                            glVertex3d(50*i, 50*j, 0);
                            glVertex3d(50*(i+1), 50*j, 0);
                            glVertex3d(50*(i+1), 50*(j+1), 0);
                            glVertex3d(50*i, 50*(j+1), 0);
                        glEnd();
                    }
                    else if(objects[i][j] == "w") {
                        // wall
                        // not implemented yet
                    }
                }
            }
        }
        bool solid_in(double x, double y) {
            int i = (int)x / 50;
            int j = (int)y / 50;
            if (i >= objects.size() || j >= 12){
                return false;
            }
            return is_solid(objects[i][j]);
        }
    private:
        vector<vector<string> > objects;      // this matrix represent plataform

        bool is_solid(const string& object) {
            return object == "f";
        }
};

#endif
