#ifndef ITERATOR_H
#define ITERATOR_H

#include <list>
#include "board.h"

class AbstractIterator {
    protected:
        const Board* board;
        std::pair<int, int> current, end;
    public: 
        bool hasNext() const;
        std::pair<int, int> getNext();
        virtual AbstractIterator& advance() = 0;
        virtual ~AbstractIterator() {}
};

class Iterator : public AbstractIterator {
    private:
        std::list<std::pair<int, int>>::const_iterator prev, it;
    public:
        Iterator(const Board* board);
        AbstractIterator& advance();
};

#endif