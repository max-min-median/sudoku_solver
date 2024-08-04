# __sudoku_solver__
Finally my own Sudoku solver!
Why? Because every hobbyist coder needs to have their own.
I mean, it's a rite of passage. Even our [ex-Prime Minister has made one](http://bit.ly/1zAXbua). 
(Also, it was one of the hard Leetcode problems, but I still don't like your C interface, Leetcode!)

## Installation
It's a single C source file. Compile and run.

## Instructions
Enter a string representing the board.
- Strings should either be 81 chars long (with no separators between lines), or 89 chars long.
- You have flexibility in the characters used to indicate empty cells or line separators, however...
- Ensure that the character used to indicate empty cells is consistent throughout.
- Ensure that the character used for the (optional) line separator is consistent throughout.

✅ `53..7....6..195....98....6.8...6...34..8.3..17...2...6.6....28....419..5....8..79`

✅ `53..7..../6..195.../.98....6./8...6...3/4..8.3..1/7...2...6/.6....28./...419..5/....8..79`

✅ `53oo7oooo+6oo195ooo+o98oooo6o+8ooo6ooo3+4oo8o3oo1+7ooo2ooo6+o6oooo28o+ooo419oo5+oooo8oo79`

❌ `53.,7.,..6..195..,.98..,.6.8...6...34..8.3..17...2...6.6....28....419..5....8..79`

❌ `53..7..../6..195...\.98....6./8...6...3\4..8.3..1/7...2...6\.6....28./...419..5\....8..79`
