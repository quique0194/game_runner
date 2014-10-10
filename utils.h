#ifndef UTILS_H
#define UTILS_H

#include <vector>
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

#endif
