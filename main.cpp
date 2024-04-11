#include "Parser.h"
#include "Sandpile.h"
#include "Image.h"
#include "Timer.h"

int main(int argc, char* argv[]) {
    Timer timer;
    Options new_options = ParseArgs(argc, argv);
    PrintImage(ParseInitialSandPile(new_options, timer), new_options.file_output, timer);
}