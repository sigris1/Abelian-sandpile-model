//
// Created by sigris on 06.11.2023.
//

#include "Parser.h"

const char* kShortFreqArgument = "-f";
const char* kLongFreqArgument = "--freq=";
const char* kShortMaxIterArgument = "-m";
const char* kLongMaxIterArgument = "--max-iter=";
const char* kShortInputArgument = "-i";
const char* kLongInputArgument = "--input=";
const char* kShortOutputArgument = "-o";
const char* kLongOutputArgument = "--output=";
const std::size_t kFreqShift = std::strlen("--freq=");
const std::size_t kIterShift = std::strlen("--max-iter=");
const std::size_t kInputShift = std::strlen("--input=");
const std::size_t kOutputShift = std::strlen("output=");

Options ParseArgs(int argc, char* argv[]) {
    Options options;
    char * end;
    char* str;
    char* next_str;
    for (int i = 1; i < argc; ++i) {
        str = argv[i];
        if (i < argc - 1) {
            next_str = argv[i + 1];
        }
        if (strcmp(str, kShortFreqArgument) == 0) {
            ++i;
            options.freq = strtol(next_str, &end, 10);
        } else if (strcmp(str, kShortMaxIterArgument) == 0){
            ++i;
            options.max_iter = strtol(next_str, &end, 10);
        } else if (strcmp(str, kShortOutputArgument) == 0) {
            ++i;
            options.file_output = next_str;
        } else if (strcmp(str, kShortInputArgument) == 0) {
            ++i;
            options.file_input = next_str;
        } else if (strncmp(str, kLongMaxIterArgument, kIterShift) == 0) {
            options.max_iter = strtol(str + kIterShift, &end, 10);
        } else if (strncmp(str, kLongFreqArgument, kFreqShift) == 0){
            options.freq = strtol(str + kFreqShift, &end, 10);
        } else if (strncmp(str, kLongOutputArgument, kOutputShift) == 0){
            options.file_output = str + kOutputShift;
        } else if (strncmp(str, kLongInputArgument, kInputShift) == 0){
            options.file_input = str + kInputShift;
        }
    }
    return options;
}