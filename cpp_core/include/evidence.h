#ifndef EVIDENCE_H
#define EVIDENCE

#include "board.h"
#include "iterator.h"
#include <list>
#include <unordered_map>
#include <set>
#include <algorithm>

struct Proof {
    const std::pair<int, int> move;
    Proof(const std::pair<int, int> move): move(move) {}
};

struct BoardEqual {
    bool operator()(const Board* lsh, const Board* rhs) const;
};

struct BoardHash {
    size_t operator()(const Board* board) const;
};

class Evidence {
protected:
    IteratorCode code;
    std::unordered_map<const Board*, Proof*, BoardHash, BoardEqual> memory;
public:
    Evidence(IteratorCode code);
    Proof* get(const Board* board);
    ~Evidence();
};

#endif