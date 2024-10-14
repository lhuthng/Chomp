 #include "pattern.h"

using namespace std;

bool Square::check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const {
    auto& generators = board->get_generators();
    if (generators.size() >= 2) {
        auto first = generators.begin(), last = prev(generators.end());
        if (first->first == last->second) {
            if (!board->contains({1, 1})) {
                *proof = nullptr;
                return true;
            }
            *proof = new Proof({1, 1});
            return true;
        }
        else if (generators.size() == 3 && *next(first) == make_pair(1, 1)) {
            if (first->first > last->second) 
                *proof = new Proof({last->second, 0});
            else 
                *proof = new Proof({0, first->first});
            return true;
        }
    }
    return false;
}

bool TwoRowPattern::check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const {
    auto& generators = board->get_generators();
    if (prev(generators.end())->second != 2 || generators.size() != 3) return false;
    auto it = generators.begin();
    const int first = (it++)->first, second = (it++)->first;
    if (first - second == 1) {
        *proof = nullptr;
        return true;
    }
    return false;
}

bool ThreeRowPattern::check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const {
    auto& generators = board->get_generators();
    if (generators.size() == 4 && prev(generators.end())->second == 3) {
        auto it = generators.begin();
        const int first = (it++)->first, second = (it++)->first, third = (it++)->first;
        if (third == r && second >= q && first - second == p - q) {
            *proof = nullptr;
            return true;
        }
    }
    else if (prev(generators.end())->second == 2) {
        auto first = generators.begin(), second = next(first);
        if (second->second == 2) {
            *proof = new Proof({first->first - 1, 1});
            return true;
        }
        else if (second->first < first->first - 1) {
            *proof = new Proof({second->first + 1, 0});
            return true;
        }
    }
    return false;
}

bool _3n2nPattern::check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const {
    auto& generators = board->get_generators();
    if (generators.size() >= 2) {
        auto it = generators.end();
        int last = (--it)->second, first = generators.begin()->first;
        const pair<int, int> off = *--it;
        
        if (
            first >= 3 
            && (off.second == 0 || off.first > 2) 
            && first % 3 == 0 && last % 2 == 0 && first / 3 == last / 2
        ) {
            if (generators.size() == 3 && *next(generators.begin()) == make_pair(2, 1)) {
                if (off == make_pair(2, 1)) {
                    *proof = nullptr;
                    return true;
                }
            }
            else {
                *proof = new Proof({2, 1});
                return true;
            }
        }
    }
    return false;
}

bool _3n2n1Pattern::check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const {
    auto& generators = board->get_generators();
    if (generators.size() >= 2) {
        auto it = generators.end();
        int last = (--it)->second, first = generators.begin()->first;
        const pair<int, int> off = *--it;
        
        if (
            first >= 3 + 1
            && (off.second == 0 || off.first > 2) 
            && (first - 1) % 3 == 0 && (last - 1) % 2 == 0 && (first - 1) / 3 == (last - 1) / 2
        ) {
            if (generators.size() == 3 && *next(generators.begin()) == make_pair(2, 1)) {
                if (off == make_pair(2, 1)) {
                    *proof = nullptr;
                    return true;
                }
            }
            else {
                *proof = new Proof({2, 1});
                return true;
            }
        }
    }
    return false;
}

bool RectanglePattern::check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const {
    if (!is_required && board->get_generators().size() == 2) {
        *proof = NULL_PROOF;
        return true;
    }
    return false;
}

const list<Pattern*> patterns({
    new Square(),
    new _3n2nPattern(),
    new _3n2n1Pattern(),
    new TwoRowPattern(),
    new ThreeRowPattern(4, 2, 2),
    new ThreeRowPattern(11, 7, 5),
    new ThreeRowPattern(15, 10, 7),
    new ThreeRowPattern(18, 12, 9),
    new ThreeRowPattern(26, 19, 11),
    new ThreeRowPattern(30, 21, 14),
    new ThreeRowPattern(35, 24, 17),

    new RectanglePattern(),
});

bool get_from_pattern(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) {
    auto generators = board->get_generators();
    if (board == Board::EMPTY) {
        *proof = new Proof({0, 0});
        return true;
    }
    else if (board == Board::POISON) {
        *proof = nullptr;
        return true;
    }
    else if (generators.size() == 2) {
        const pair<int, int>& first = *generators.begin();
        const pair<int, int>& second = *next(generators.begin());
        if (second == make_pair(0, 1)) {
            *proof = new Proof({1, 0});
            return true;
        }
    }
    for (const Pattern* pattern: patterns) {
        if (pattern->check(board, proof, is_required, NULL_PROOF)) return true;
    }
    return false;
}
