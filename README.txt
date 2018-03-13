Joseph Min
- Changed Board class framework to be more intuitive by using
  a 2D integer array rather than bitwise.
- Improved Board::doMove by removing redundant calculations. This
  was achieved by creating a new struct, Capture, and writing
  Board::checkMoveCapture, which checks whether a move is valid
  and keeps track of what pieces are captured due to that move.

Nivetha Karthikeyan
-
-

Improvements (both implemented and attempted)
- Attempted to multithread the recursive calculation of the minimum
  score. Rolled back due to problems with dividing workload equally
  to number of cores.
- Improved inefficient code in Board::doMove that caused it to
  double-check whether a move is valid (once when checking if the move
  is a valid move and once when calculating the captures). Instead,
  replaced inefficient call with Board::checkMoveCapture, which checks
  whether a move is valid and keeps track of what captures are made due
  to that move.
- Changed Board class to hold a 2D integer array representation of the
  board instead of bitwise. Though it is unclear how much of an impact
  this change has on the performance, it makes it easier for us
  to code because a 2D integer array is more intuitive.
- Player calculates the score by a heuristic algorithm that prefers
  moves in the edge of the board and discouraging moves adjacent to
  the corners.
