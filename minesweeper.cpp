#include <iostream>
#include <cstdlib>

using namespace std;

void createBoard(int board[], size_t width, size_t height, size_t count) {
    //randomly generate spaces for mines
    //mine cannot be placed on start position
    //duplicate mines cannot be placed, either randomly choose a new spot or choose an adjacent spot
}

int main() {
    //declare parameters of board
    const size_t width = 15;
    const size_t height = 15;
    size_t minecount = 5;

    //create an array to store mine locations & cell values, and an array that copies from the first for output
    int internal[height][width];
    int external[height][width];
    //consider start to be (8, 8)

    return 0;
}