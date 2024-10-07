from board import *

class Evidence():
    def __init__(self, only_one=False, trace_back_fn=rev_sub):
        self.memory = dict()
        self.memory[()] = ()
        self.memory[(1,)] = False
        self.only_one=only_one
        self.trace_back_fn = trace_back_fn

    def get(self, board: tuple):
        board = sim(board)
        if board in self.memory: return self.memory[board]
        found = False
        for child in self.trace_back_fn(board):
            if self.get(child) == False:
                found = True
                if self.only_one:
                    self.memory[board] = child
                    break
                else: 
                    self.memory[board] = self.memory.get(board, [])
                    self.memory[board].append(child)
        if not found:
            self.memory[board] = False
        return self.memory[board]

class EvidencePlus(Evidence):
    def __init__(self, trace_back_fn=rev_sub):
        self.skipped = 0
        super().__init__(False, trace_back_fn)
    def get(self, board: tuple):
        board = sim(board)
        if board in self.memory: return self.memory[board]
        pattern_result = self.find_pattern(board)
        if pattern_result is not None:
            self.skipped += 1
            self.memory[board] = pattern_result
            return pattern_result
        else:
            return super().get(board)
        
    @staticmethod
    def find_pattern(sboard) -> object:
        length = len(sboard)
        # 1-row chomp
        if length == 1:
            if sboard[0] == 1:
                return False
            else:
                return (1, )
            
        # 2-row chomp
        if length == 2:
            p, q = sboard
            if p == q:
                return trim((p, q-1))
            elif p == q + 1:
                return False
            else:
                return trim((q + 1, q))
        
        # 3-row chomp
        if length == 3:
            p, q, r = sboard
            if p - 1 == q == r:
                return (p, q)
            # ********
            # ******
            # *
        # extended-2-row chomp
        if length > 2 and sboard[-1] == 1 and sboard[-2] >= 2:
            return (length, length - 1)
            
            
        # symetric chomp
        if length > 1 and sboard[0] == length and sboard == flip(sboard):
            if sboard[1] > 1:
                return (length, ) + (1, ) * (length - 1)
            else:
                return False
        if all(x == 1 for x in sboard[1:]):
            return (length, ) + (1, ) * (length - 1)

        return None