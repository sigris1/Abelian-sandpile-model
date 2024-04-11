
#include "Image.h"
#include "Sandpile.h"
#include "ostream"
#include "cstring"

const int16_t kFileHeaderSize = 14;
const int16_t kInformationHeaderSize = 40;
const int16_t kPaletteSize = 20;
const int16_t kBinaryShift = 16;
const int16_t kMaxPossibleColor = 4;
const int16_t kMaxNonDispersibleValue = 3;

void PrintImage(Sandpile sandpile, char * file_out, Timer timer){
    char name[100];
    name[0] = '"';
    std::string str = (std::to_string(timer.number));
    char num[str.length()];
    for (int i = 0; i < str.length() + 1; i++) {
        num[i] = str[i];
    }
    strcpy(name,file_out);
    strcat(name, num);
    strcat(name, ".bmp");
    strcat(name, reinterpret_cast<const char *>(""));
    std::FILE * file_in = fopen( name, "w");
    uint16_t hight = sandpile.topmost_y - sandpile.botmost_y;
    uint16_t length = sandpile.rightmost_x - sandpile.leftmost_x;
    const uint64_t kPaddingLength = (8 - (length) % 8) % 8;
    const uint16_t kFullLength = length + kPaddingLength;
    const uint16_t kFileSize = kFileHeaderSize + kInformationHeaderSize + kPaletteSize + hight * kFullLength;
    uint8_t fileHeader[kFileHeaderSize];
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';

    fileHeader[2] = kFileSize;
    fileHeader[3] = kFileSize >> 8;
    fileHeader[4] = kFileSize >> 16;
    fileHeader[5] = kFileSize >> 24;

    fileHeader[6] = 0;
    fileHeader[7] = 0;
    fileHeader[8] = 0;
    fileHeader[9] = 0;

    fileHeader[10] = kFileHeaderSize + kInformationHeaderSize + kPaletteSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    uint8_t informationHeader[kInformationHeaderSize];
    informationHeader[0] = kInformationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;

    informationHeader[4] =  length;
    informationHeader[5] =  length >> 8;
    informationHeader[6] =  length >> 16;
    informationHeader[7] =  length >> 24;

    informationHeader[8] = hight;
    informationHeader[9] = hight >> 8;
    informationHeader[10] = hight >> 16;
    informationHeader[11] = hight >> 24;

    informationHeader[12] = 1;
    informationHeader[13] = 0;
    informationHeader[14] = 4;
    informationHeader[15] = 0;
    for (int i = 16; i < 40; ++i){
        informationHeader[i] = 0;
    }
    informationHeader[32] = 5;

    Color color_palette[5] = {
            {255, 255, 255, 0},
            {0, 255, 0, 0},
            {0, 255, 255, 0},
            {255, 0, 255, 0},
            {0, 0, 0, 0}
    };
    fwrite(fileHeader, sizeof(fileHeader), 1, file_in);
    fwrite(informationHeader, sizeof(informationHeader), 1, file_in);
    fwrite(color_palette, kPaletteSize, 1, file_in);
    int8_t pixels;
    uint16_t first_color;
    uint16_t second_color;
    for (uint16_t j = 0; j < hight; ++j) {
        for (uint16_t i = 0; i < kFullLength; i += 2) {
            if (i >= length) {
                first_color = 0;
                second_color = 0;
            } else if (i + 1 == length) {
                first_color = sandpile.sandpile[j][i];
                second_color = 0;
            } else {
                first_color = sandpile.sandpile[j][i];
                second_color = sandpile.sandpile[j][i + 1];
            }
            if (first_color > kMaxNonDispersibleValue) {
                first_color = kMaxPossibleColor;
            }
            if (second_color > kMaxNonDispersibleValue) {
                second_color = kMaxPossibleColor;
            }
            pixels = first_color * kBinaryShift + second_color;
            fwrite(&pixels, sizeof(pixels), 1, file_in);
        }
    }
    fclose(file_in);
};
