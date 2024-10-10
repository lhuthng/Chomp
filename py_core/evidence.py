from .board import *
from .iterator import *

class Evidence():
    def __init__(self, counter_move=False, only_one=False, trace_back_fn=iterator):
        self.memory = dict()
        self.memory[()] = ()
        self.memory[(1,)] = False
        self.only_one=only_one
        self.trace_back_fn = trace_back_fn
        self.counter_move = counter_move

    def get(self, board):
        if board.data in self.memory: return self.memory[board.data]
        found = False
        for move in self.trace_back_fn(board):
            child = board.consume(*move)
            if self.get(child) == False:
                found = True
                if board.flipped: move = move[::-1]
                counter = move if self.counter_move else child.data
                if self.only_one:
                    self.memory[board.data] = counter
                    break
                else: 
                    self.memory[board.data] = self.memory.get(board.data, [])
                    self.memory[board.data].append(counter)
        if not found:
            self.memory[board.data] = False
        return self.memory[board.data]

# class EvidencePlus(Evidence):
#     def __init__(self, trace_back_fn=iterator):
#         self.skipped = 0
#         super().__init__(False, trace_back_fn)
#     def get(self, board: tuple):
#         board = sim(board)
#         if board in self.memory: return self.memory[board]
#         pattern_result = self.find_pattern(board)
#         if pattern_result is not None:
#             self.skipped += 1
#             self.memory[board] = pattern_result
#             return pattern_result
#         else:
#             return super().get(board)
        
#     @staticmethod
#     def find_pattern(sboard) -> object:
#         length = len(sboard)
#         # 1-row chomp
#         if length == 1:
#             if sboard[0] == 1:
#                 return False
#             else:
#                 return (1, )
            
#         # 2-row chomp
#         if length == 2:
#             p, q = sboard
#             if p == q:
#                 return trim((p, q-1))
#             elif p == q + 1:
#                 return False
#             else:
#                 return trim((q + 1, q))
        
#         # 3-row chomp
#         if length == 3:
#             p, q, r = sboard
#             if p - 1 == q == r:
#                 return (p, q)
#             # ********
#             # ******
#             # *
#         # extended-2-row chomp
#         if length > 2 and sboard[-1] == 1 and sboard[-2] >= 2:
#             return (length, length - 1)
            
            
#         # symetric chomp
#         if length > 1 and sboard[0] == length and sboard == flip(sboard):
#             if sboard[1] > 1:
#                 return (length, ) + (1, ) * (length - 1)
#             else:
#                 return False
#         if all(x == 1 for x in sboard[1:]):
#             return (length, ) + (1, ) * (length - 1)

#         return None