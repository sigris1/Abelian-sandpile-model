
#include "Sandpile.h"
#include "fstream"
#include "string"
#include "cstring"
#include "cstdint"
#include "Image.h"
#include "Parser.h"
#include "Timer.h"

const uint16_t kMaxIndex = INT16_MIN;
const uint16_t kMinIndex = INT16_MAX;
const uint16_t kMaxNonDispersibleValue = 3;
const uint16_t kTwoWrongBorder = 2;
const uint16_t kOneWrongBorder = 1;
const uint16_t kZeroWrongBorder = 0;
const uint16_t kPathInputLength = 50;
const uint16_t kOneDisperseOperation = 4;

void PushLeft(Sandpile& sandpile){
    for (int i = 0; i < sandpile.topmost_y - sandpile.botmost_y; ++i){
        sandpile.sandpile[i][0] = 0;
    }
}

void PushRight(Sandpile& sandpile){
    for (int i = 0; i < sandpile.topmost_y - sandpile.botmost_y; ++i){
        sandpile.sandpile[i][sandpile.rightmost_x - sandpile.leftmost_x - 1] = 0;
    }
}

void PushTop(Sandpile& sandpile){
    for (int j = 0; j < sandpile.rightmost_x - sandpile.leftmost_x; ++j){
        sandpile.sandpile[0][j] = 0;
    }
}

void PushBot(Sandpile& sandpile){
    for (int j = 0; j < sandpile.rightmost_x - sandpile.leftmost_x; ++j){
        sandpile.sandpile[sandpile.topmost_y - sandpile.botmost_y - 1][j] = 0;
    }
}

void PushWrongBorders(Sandpile& sandpile, bool is_wrong_left_border, bool is_wrong_right_border, bool is_wrong_bot_border, bool is_wrong_top_border){
    if (is_wrong_right_border) {
        PushRight(sandpile);
    }
    if (is_wrong_bot_border) {
        PushBot(sandpile);
    }
    if (is_wrong_top_border) {
        PushTop(sandpile);
    }
    if (is_wrong_left_border){
        PushLeft(sandpile);
    }
}

uint16_t** CreateBinarySandpile(Sandpile sandpile){
    sandpile.sandpile = new uint16_t * [sandpile.topmost_y - sandpile.botmost_y];
    for (int i = 0; i < sandpile.topmost_y - sandpile.botmost_y; ++i){
        sandpile.sandpile[i] = new uint16_t [sandpile.rightmost_x - sandpile.leftmost_x];
    }
    return sandpile.sandpile;
}

uint16_t** SetMeaningToZero(Sandpile sandpile){
    for (uint64_t i = 0; i < sandpile.topmost_y - sandpile.botmost_y; ++i){
        for (uint64_t j = 0; j < sandpile.rightmost_x - sandpile.leftmost_x; ++j){
            sandpile.sandpile[i][j] = 0;
        }
    }
    return sandpile.sandpile;
}

uint16_t** SetMeaningToSandpilesMeaning(Sandpile first, Sandpile second, int shift_i, int shift_j, int shift_pos_i, int shift_pos_j){
    for (int i = 0; i < first.topmost_y - first.botmost_y - shift_i; ++i){
        for (int j = 0; j < first.rightmost_x - first.leftmost_x - shift_j; ++j){
            first.sandpile[i + shift_pos_i][j + shift_pos_j] = second.sandpile[i][j];
        }
    }
    return first.sandpile;
}

uint16_t** ProcessWrongBorders(Sandpile now_sandpile, Sandpile Fsandpile, bool is_wrong_left_border, bool is_wrong_top_border, bool is_wrong_bot_border, bool is_wrong_right_border, int16_t count_wrong_horizontal_border, int16_t count_wrong_vertical_border) {
    now_sandpile.sandpile = CreateBinarySandpile(now_sandpile);
    now_sandpile.sandpile = SetMeaningToZero(now_sandpile);
    if (count_wrong_vertical_border == kTwoWrongBorder && count_wrong_horizontal_border == kTwoWrongBorder){
        PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
        now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 2, 2, 1, 1);
    } else if (count_wrong_vertical_border == kTwoWrongBorder && count_wrong_horizontal_border == kOneWrongBorder){
        if (is_wrong_left_border){
            PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
            now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 2, 1, 1, 1);
        } else {
            PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
            now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 2, 1, 1, 0);
        }
    } else if (count_wrong_vertical_border == kTwoWrongBorder && count_wrong_horizontal_border == kZeroWrongBorder){
        PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
        now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 2, 0, 1, 0);
    } else if (count_wrong_vertical_border == kOneWrongBorder && count_wrong_horizontal_border == kTwoWrongBorder){
        if (is_wrong_top_border){
            PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
            now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 1, 2, 1, 1);
        } else {
            PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
            now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 1, 2, 0, 1);
        }
    } else if (count_wrong_vertical_border == kOneWrongBorder && count_wrong_horizontal_border == kOneWrongBorder){
        if (is_wrong_top_border){
            if (is_wrong_left_border){
                PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
                now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 1, 1, 1, 1);
            } else {
                PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
                now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 1, 1, 1, 0);
            }
        } else {
            if (is_wrong_left_border){
                PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
                now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 1, 1, 0, 1);
            } else {
                PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
                now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 1, 1, 0, 0);
            }
        }
    } else if (count_wrong_vertical_border == kOneWrongBorder && count_wrong_horizontal_border == kZeroWrongBorder){
        if (is_wrong_top_border){
            PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
            now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 1, 0, 1, 0);
        } else {
            PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
            now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 1, 0, 0, 0);
        }
    } else if (count_wrong_vertical_border == kZeroWrongBorder && count_wrong_horizontal_border == kTwoWrongBorder){
        PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
        now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 0, 2, 0, 1);
    } else if (count_wrong_vertical_border == kZeroWrongBorder && count_wrong_horizontal_border == kOneWrongBorder){
        if (is_wrong_left_border){
            PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
            now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 0, 1, 0, 1);
        } else {
            PushWrongBorders(now_sandpile, is_wrong_left_border, is_wrong_right_border, is_wrong_bot_border, is_wrong_top_border);
            now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 0, 1, 0, 0);
        }
    } else if (count_wrong_vertical_border == kZeroWrongBorder && count_wrong_horizontal_border == kZeroWrongBorder){
        now_sandpile.sandpile = SetMeaningToSandpilesMeaning(now_sandpile, Fsandpile, 0, 0, 0, 0);
    }
    return now_sandpile.sandpile;
}

Sandpile PrintSandPile(Sandpile Fsandpile, Options& options, Timer& timer){
    bool is_wrong_left_border = false;
    bool is_wrong_top_border = false;
    bool is_wrong_bot_border = false;
    bool is_wrong_right_border = false;
    int16_t count_wrong_horizontal_border = 0;
    int16_t count_wrong_vertical_border = 0;
    for (int i = 0; i < Fsandpile.topmost_y - Fsandpile.botmost_y; ++i) {
        if (Fsandpile.sandpile[i][0] > kMaxNonDispersibleValue){
            is_wrong_left_border = true;
        }
        if (Fsandpile.sandpile[i][Fsandpile.rightmost_x - Fsandpile.leftmost_x - 1] > kMaxNonDispersibleValue){
            is_wrong_right_border = true;
        }
    }
    for (int j = 0; j < Fsandpile.rightmost_x - Fsandpile.leftmost_x; ++j) {
        if (Fsandpile.sandpile[0][j] > kMaxNonDispersibleValue) {
            is_wrong_top_border = true;
        }
        if (Fsandpile.sandpile[Fsandpile.topmost_y - Fsandpile.botmost_y - 1][j] > kMaxNonDispersibleValue) {
            is_wrong_bot_border = true;
        }
    }
    Sandpile now_sandpile;
    now_sandpile.topmost_y = Fsandpile.topmost_y;
    now_sandpile.botmost_y = Fsandpile.botmost_y;
    now_sandpile.rightmost_x = Fsandpile.rightmost_x;
    now_sandpile.leftmost_x = Fsandpile.leftmost_x;
    if (is_wrong_left_border){
        ++now_sandpile.rightmost_x;
        ++count_wrong_horizontal_border;
    }
    if (is_wrong_right_border){
        ++now_sandpile.rightmost_x;
        ++count_wrong_horizontal_border;
    }
    if (is_wrong_top_border){
        ++now_sandpile.topmost_y;
        ++count_wrong_vertical_border;
    }
    if (is_wrong_bot_border){
        ++now_sandpile.topmost_y;
        ++count_wrong_vertical_border;
    }
    now_sandpile.sandpile = ProcessWrongBorders(now_sandpile, Fsandpile, is_wrong_left_border, is_wrong_top_border, is_wrong_bot_border, is_wrong_right_border, count_wrong_horizontal_border, count_wrong_vertical_border);
    uint16_t ** checking_sandpile = CreateBinarySandpile(now_sandpile);
    for (uint16_t i = 0; i < now_sandpile.topmost_y - now_sandpile.botmost_y; ++i){
        for (uint16_t j = 0; j < now_sandpile.rightmost_x - now_sandpile.leftmost_x; ++j){
            checking_sandpile[i][j] = now_sandpile.sandpile[i][j];
        }
    }
    for (int i = 1; i < now_sandpile.topmost_y - now_sandpile.botmost_y - 1; ++i){
        for (int j = 1; j < now_sandpile.rightmost_x - now_sandpile.leftmost_x - 1; ++j){
            if (checking_sandpile[i][j] > kMaxNonDispersibleValue){
                now_sandpile.sandpile[i][j] -= kOneDisperseOperation;
                ++now_sandpile.sandpile[i + 1][j];
                ++now_sandpile.sandpile[i - 1][j];
                ++now_sandpile.sandpile[i][j + 1];
                ++now_sandpile.sandpile[i][j - 1];
            }
        }
    }
    --options.max_iter;
    timer.position++;
    for (int16_t i = 0; i < now_sandpile.topmost_y - now_sandpile.botmost_y; ++i){
        delete(checking_sandpile[i]);
    }
    for (int16_t i = 0; i < Fsandpile.topmost_y - Fsandpile.botmost_y; ++i){
        delete(Fsandpile.sandpile[i]);
    }
    for (int i = 0; i < now_sandpile.topmost_y - now_sandpile.botmost_y; ++i){
        for (int j = 0; j < now_sandpile.rightmost_x - now_sandpile.leftmost_x; ++j){
            if (now_sandpile.sandpile[i][j] > kMaxNonDispersibleValue){
                ++now_sandpile.count;
            }
        }
    }
    return now_sandpile;
}

Sandpile ParseInitialSandPile(Options options, Timer& timer){
    char name[kPathInputLength];
    name[0] = '"';
    strcpy(name, options.file_input);
    strcat(name, reinterpret_cast<const char *>(""));
    std::fstream file(name);
    int16_t leftmost_x = kMinIndex;
    int16_t rightmost_x = kMaxIndex;
    int16_t topmost_y = kMaxIndex;
    int16_t botmost_y = kMinIndex;
    int64_t temp;
    uint64_t position = 0;
    while (file >> temp) {
        if (position % 3 == 0) {
            if (temp > rightmost_x) {
                rightmost_x = temp;
            }
            if (temp < leftmost_x) {
                leftmost_x = temp;
            }
        } else if (position % 3 == 1) {
            if (temp > topmost_y) {
                topmost_y = temp;
            }
            if (temp < botmost_y) {
                botmost_y = temp;
            }
        }
        position++;
    }
    file.close();
    ++topmost_y;
    ++rightmost_x;
    uint16_t** sandpile = new uint16_t * [topmost_y - botmost_y];
    for (int i = 0; i < topmost_y - botmost_y; ++i){
        sandpile[i] = new uint16_t [rightmost_x - leftmost_x];
    }
    for (uint64_t i = 0; i < topmost_y - botmost_y; ++i){
        for (uint64_t j = 0; j < rightmost_x - leftmost_x; ++j){
            sandpile[i][j] = 0;
        }
    }
    position = 0;
    std::fstream file1(name);
    int16_t position_x = 0;
    int16_t position_y = 0;
    uint64_t sands = 0;
    uint64_t count  = 0;
    while (file1 >> temp) {
        if (position % 3 == 0) {
            position_x = temp - leftmost_x;
        } else if (position % 3 == 1) {
            position_y = temp - botmost_y;
        } else {
            sands = temp;
            sandpile[position_y][position_x] = sands;
            if (sands > kMaxNonDispersibleValue){
                ++count;
            }
        }
        position++;
    }
    file.close();
    Sandpile start_sandpile;
    start_sandpile.sandpile = sandpile;
    start_sandpile.rightmost_x = rightmost_x;
    start_sandpile.topmost_y = topmost_y;
    start_sandpile.botmost_y = botmost_y;
    start_sandpile.leftmost_x = leftmost_x;
    Sandpile final_sandpile = PrintSandPile(start_sandpile, options, timer);
    while (final_sandpile.count > 0 && options.max_iter > 0) {
        if (timer.position % options.freq == 0) {
            timer.number++;
            PrintImage(final_sandpile, options.file_output, timer);
        }
        final_sandpile = PrintSandPile(final_sandpile, options, timer);
    }
    return final_sandpile;
}
