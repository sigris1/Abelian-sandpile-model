#pragma once

#include "iostream"
#include "fstream"
#include "string"
#include "cmath"
#include "cstring"
#include "cstdint"

struct Options {
    uint64_t max_iter;
    uint64_t freq;
    char* file_input;
    char* file_output;
};

Options ParseArgs(int argc, char* argv[]);
