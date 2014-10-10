#ifndef FILE_H
#define FILE_H

#include <fstream>
using std::ifstream;
#include <sstream>
using std::stringstream;
#include <string>
using std::string;
using std::getline;
#include <vector>
using std::vector;

class File: public ifstream {
    public:
        explicit File(const string& filename): ifstream(filename.c_str()) {
        }

        string read_all() {
            stringstream buffer;
            buffer << rdbuf();
            return buffer.str();
        }

        bool read_line(string& line) {
            return ::getline(*this, line);
        }

        void read_lines(vector<string>& lines) {
            string line;
            while (::getline(*this, line)) {
                lines.push_back(line);
            }
        }

        vector<string> read_lines() {
            vector<string> lines;
            read_lines(lines);
            return lines;
        }
};

#endif
