#ifndef BOARD_H
#define BOARD_H

#include <list>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <iostream>
#include <string>

struct ListPairEqual {
    bool operator()(const std::list<std::pair<int, int>>& lhs, const std::list<std::pair<int, int>>& rhs) const;
};

struct PairListHash {
    size_t operator()(const std::list<std::pair<int, int>>& list) const;
};

class Board {
private:
    const std::list<std::pair<int, int>>* p_generators;
    static std::unordered_map<std::list<std::pair<int, int>>, Board*, PairListHash, ListPairEqual> boards;
    Board(const std::list<std::pair<int, int>> * p_generators) {
        this->p_generators = p_generators;
    }
public:
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
    const std::list<std::pair<int, int>>& get_generators() const;
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
    bool operator<(const Board& other) const;
    template <typename GenIter1, typename GenIter2, typename GetFunc>
    static bool check(GenIter1 begin1, GenIter1 end1, GenIter2 begin2, GenIter2 end2, GetFunc& first, GetFunc& second);
    bool can_reach(const Board* target) const;
    const Board* consume(const std::pair<int, int> &position) const;
    static const Board* get_board(int width, int height);
    static const Board* get_board(std::string input);
    static const Board* get_board(std::list<std::pair<int, int>> generators);
    static void clean_up();
    static void flip(std::list<std::pair<int, int>>& list);
    static void simplify(std::list<std::pair<int, int>>& generators);
};

#endif