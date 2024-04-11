#pragma once

#include "cstdint"
#include "Parser.h"
#include "Timer.h"

struct Sandpile{
    uint16_t rightmost_x = 0;
    uint16_t leftmost_x = 0;
    uint16_t topmost_y = 0;
    uint16_t botmost_y = 0;
    uint16_t** sandpile = new uint16_t * [topmost_y - botmost_y];
    uint64_t  count  = 0;
};

void PushLeft(Sandpile& sandpile);

void PushRight(Sandpile& sandpile);

void PushTop(Sandpile& sandpile);

void PushBot(Sandpile& sandpile);

void PushWrongBorders(Sandpile& sandpile, bool is_wrong_left_border, bool is_wrong_right_border, bool is_wrong_bot_border, bool is_wrong_top_border);

uint16_t** ProcessWrongBorders(Sandpile now_sandpile, Sandpile Fsandpile, bool is_wrong_left_border, bool is_wrong_top_border, bool is_wrong_bot_border, bool is_wrong_right_border, int16_t count_wrong_horizontal_border, int16_t count_wrong_vertical_border);

uint16_t** CreateBinarySandpile(Sandpile sandpile);

uint16_t** SetMeaningToZero(Sandpile sandpile);

uint16_t** SetMeaningToSandpilesMeaning(Sandpile first, Sandpile second, int shift_i, int shift_j, int shift_pos_i, int shift_pos_j);

Sandpile PrintSandPile(Sandpile sandpile, Options& options, Timer& timer);

Sandpile ParseInitialSandPile(Options options, Timer& timer);

