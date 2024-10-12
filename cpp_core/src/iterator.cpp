#include "iterator.h"
#include <iostream>

using namespace std;


bool AbstractIterator::hasNext() const {
    return current != end;
}

pair<int, int> AbstractIterator::getNext() {
    pair<int, int> result = current;
    advance();
    return result;
}

AbstractIterator::AbstractIterator(const Board* board) {
    this->board = board;
}

Iterator::Iterator(const Board* board) : AbstractIterator(board) {
    init();
}

AbstractIterator& Iterator::advance() {
    if (it != board->get_generators().end()) {
        if (current.first < prev->first - 1) {
            current.first += 1;
        }
        else {
            current.first = 0;
            current.second += 1;
            if (current.second > it->second - 1) prev = it++;
        }
    }
    return *this;
}

void Iterator::init() {
    it = this->board->get_generators().begin();
    prev = it++;
    end = {0, std::prev(board->get_generators().end())->second};
    if (it == board->get_generators().end()) current = {0, 1};
    else current = {1, 0};
}
