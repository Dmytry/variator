// Copyright (c) 2006-2022 Dmitry Lavrov.

// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

std::string ReplaceStr(std::string s, const std::string &what,
                       const std::string &by_what) {
    size_t pos = 0;
    while (pos < s.size()) {
        pos = s.find(what, pos);
        if (pos == std::string::npos) break;
        s.replace(pos, what.size(), by_what);
        pos += by_what.size();
    }
    return s;
};

int ProcessFile(std::istream &in, const std::string &name, int variation) {
    int variations = 1;
    std::stringstream out;
    std::string variation_name;
    int v = variation;
    while (in.good() && (!in.eof())) {
        char c = in.get();
        if (!in.good()) break;
        if (c == '`') {
            std::vector<std::string> values;
            std::string value;
            c = in.get();
            while (in.good() && (c != '`')) {
                if (c == '|') {
                    values.push_back(value);
                    value.clear();
                } else {
                    value += c;
                }
                c = in.get();
            }
            values.push_back(value);
            if (!in.good()) {
                std::cout << "ERROR: unclosed `" << std::endl;
                break;
            }
            int i = v % values.size();
            v = v / values.size();
            variations *= values.size();
            out << values[i];
            std::stringstream num_s;
            num_s << i;
            if (!variation_name.empty()) variation_name += "_";
            variation_name += num_s.str();
        } else {
            out << c;
        }
    }
    std::string out_filename = ReplaceStr(name, "-i-", variation_name);
    if (out_filename == name) out_filename = name + "_" + variation_name;
    std::cout << "Writing file " << out_filename << std::endl;
    std::ofstream out_file(out_filename.c_str());
    out_file << out.str();
    return variations;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Usage example: Variator scene.txt scene_-i-.txt";
        return 1;
    }
    std::string in_filename = argv[1];
    std::string out_filename = argv[2];
    int variation = 0;
    int variations = 1;
    while (variation < variations) {
        std::ifstream in_file(in_filename.c_str());
        variations = 1;
        variations = ProcessFile(in_file, out_filename, variation);
        variation++;
    }
    return 0;
}
