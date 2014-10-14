#ifndef UTILS_H
#define UTILS_H
#include <stdio.h> 
#include <stdlib.h> 
#include <vector>
#include <cstring>
#include <iostream>
using std::vector;
#include <string>
using std::string;

void split(const string& orig, const char delim, vector<string>& ret) {
    ret.push_back("");
    for (int i = 0; i < orig.size(); ++i) {
        if (orig[i] == delim) {
            ret.push_back("");
        } else {
            ret[ret.size()-1] += orig[i];
        }
    }
}

void print(int x, int y,int z, char *string)
{
    //set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x,y);
    //get the length of the string to display
    int len = (int) strlen(string);
    //loop to display character by character
    for (int i = 0; i < len; i++) 
    {
        // glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
    }
}

char* int_to_char(int number)
{
  stringstream ss;
  ss << number;
  string variable = ss.str();
  string _new = "Bonus: " + variable;
  char* var = new char[_new.length()+1];
  strcpy(var,_new.c_str());
  return var;
}

#endif
