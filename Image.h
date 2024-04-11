#pragma once

#include "Sandpile.h"
#include "Parser.h"
#include "Timer.h"

struct Color {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t res = 0;

    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t res) : red(red), green(green), blue(blue), res(res) {
    }
};


void PrintImage(Sandpile sandpile, char * file_out, Timer timer);