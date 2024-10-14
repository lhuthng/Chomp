#include "evidence.h"
#include "iterator.h"
#include "pattern.h"

using namespace std;
bool BoardEqual::operator()(const Board* lst, const Board* rhs) const {
    return lst == rhs;
}

size_t BoardHash::operator()(const Board* board) const {
    return reinterpret_cast<std::size_t>(board);
}

Evidence::Evidence(IteratorCode code) : code(code) {
    memory.insert({ Board::get_board(1, 1), nullptr} );
}

Evidence::~Evidence() {
    for (auto&[fst, snd] : memory) delete snd;
    memory.clear();
}

Proof* Evidence::get(const Board* board) {
    auto it = memory.find(board);
    if (it != memory.end()) return it->second;
    Proof* proof = nullptr;
    AbstractIterator* iterator = IteratorFactory::create(code, board);
    while (iterator->hasNext()) {
        const pair<int, int> move = iterator->getNext();
        if (get(board->chomp(move)) == nullptr) {
            proof = new Proof(move);
            break;
        }
    }
    memory.insert({ board, proof });
    return proof;
}


ExtendedEvidence::ExtendedEvidence(IteratorCode code) : Evidence(code) {
    NULL_PROOF = new Proof({-1, -1});
}

ExtendedEvidence::~ExtendedEvidence() {
    delete NULL_PROOF;
}

Proof* ExtendedEvidence::get(const Board* board) {
    return get(board, true);
}


bool get_from_cache();

Proof* ExtendedEvidence::get(const Board* board, bool is_required) {
    auto it = memory.find(board);
    Proof* proof;
    if (it != memory.end()) {
        if ((proof = it->second) == NULL_PROOF && is_required) goto search;
        return proof;
    }
    if (get_from_pattern(board, &proof, is_required, NULL_PROOF)) goto skip;
search:
    {
        proof = nullptr;
        AbstractIterator* iterator = IteratorFactory::create(code, board);
        while (iterator->hasNext()) {
            const pair<int, int> move = iterator->getNext();
            if (get(board->chomp(move), false) == nullptr) {
                proof = new Proof(move);
                goto skip;
            }
        }
    }
skip:
    memory.insert({ board, proof });
    return proof;
}
