#include <iostream>
#include "board.h"
#include "iterator.h"
int main() {
    std::cout << "HELLO\n";
    Board const *board = Board::get_board("<(3,0),(1,1),(0,3)>");
    AbstractIterator *iterator = new Iterator(board);
    for (int i = 0; i < 10 && iterator->hasNext(); i++) {
        std::pair<int, int> move = iterator->getNext();
        std::cout << move.first << " " << move.second << " " << std::endl;
        move = iterator->end;
        std::cout << " " << move.first << " " << move.second << " " << std::endl;
    }
}