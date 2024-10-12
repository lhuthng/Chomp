#include "evidence.h"
#include "iterator.h"

using namespace std;
bool BoardEqual::operator()(const Board* lst, const Board* rhs) const {
    return lst == rhs;
}

size_t BoardHash::operator()(const Board* board) const {
    return reinterpret_cast<std::size_t>(board);
}

Evidence::Evidence() {
    memory.insert({ Board::get_board(1, 1), nullptr} );
}

Proof* Evidence::get(const Board* board) {
    auto it = memory.find(board);
    Proof* result = nullptr;
    if (it != memory.end()) {
        result = it->second;
    }
    else {
        Iterator iterator(board);
        while (iterator.hasNext()) {
            const pair<int, int> move = iterator.getNext();
            if (get(board->chomp(move)) == nullptr) {
                result = new Proof(move);
                memory.insert({ board, result });
                goto skip;
            }
        }
        memory.insert({ board, nullptr });
    }
skip:
    return result;
}

Evidence::~Evidence() {
    for (auto&[fst, snd] : memory) delete snd;
    memory.clear();
}