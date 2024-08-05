#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define SIZE 9

typedef uint16_t u16;
typedef uint8_t u8;

typedef struct board {
    u16 grid[SIZE][SIZE];  // Bits represent pencil markings, e.g. 110001010 indicates 1, 2, 6, or 8 are possible.
                           // Bit 10 (from right): 1 = Pencil marking, 0 = 'Fixed'
    u8 status;  // BIT 1: SOLVED, BIT 2: INSOLUBLE
} board;

board EMPTY_BOARD = {
    .grid = {[0 ... SIZE-1] = {[0 ... SIZE-1] = 0b1111111111}},  // Empty board starts with full pencil markings
    .status = 0
};

#define BIT(n) (1 << (n) - 1)
#define SOLVED BIT(1)
#define INSOLUBLE BIT(2)
#define ALLBITS ((1 << (SIZE + 1)) - 1)
#define ALL_NUM_BITS ((1 << SIZE) - 1)
#define IS_NOT_FIXED(n) ((n) & BIT(10))
#define IS_FIXED(n) (!IS_NOT_FIXED(n))
#define BITS_TO_NUM(n) (IS_FIXED(n) ? SIZE - __builtin_ctz(n) : 0)
#define NUM_TO_BITS(n) (1 << (SIZE - (n)))
#define COMPLEMENT(n) (ALLBITS ^ (n))
#define ERASE_PENCIL(a, b) (a &= COMPLEMENT(b))
#define FIX_NUMBER(brd, r, c, n) do { \
    int num = NUM_TO_BITS(n); \
    int r2 = (r) / 3 * 3, c2 = (c) / 3 * 3; \
    for (int i = 0; i < SIZE; i++) { \
        ERASE_PENCIL(brd.grid[r][i], num); \
        ERASE_PENCIL(brd.grid[i][c], num); \
        ERASE_PENCIL(brd.grid[r2 + i/3][c2 + i%3], num); \
        if (!(brd.grid[r][i] & ALL_NUM_BITS) && i != c || !(brd.grid[i][c] & ALL_NUM_BITS) && i != r || !(brd.grid[r2 + i/3][c2 + i%3] & ALL_NUM_BITS) && r != r2 + i/3 && c != c2 + i%3) { \
            brd.status |= INSOLUBLE; \
            break; \
        } \
    } \
    brd.grid[r][c] = num; \
} while (0)
#define PRINT_PENCIL(n) do { \
    for (u16 i = 1; i <= SIZE; i++) \
        if ((1 << SIZE - i) & n) putchar('0' + i); \
} while (0)
#define FOR_BOARD(R, C, code) for (int R = 0; R < SIZE; R++) for (int C = 0; C < SIZE; C++) {code}
#define FOR(i, start, end, code) for (int i = start; i < end; i++) {code}
#define PRINT_BOARD(b) do { \
    FOR_BOARD(y, x, \
        if IS_FIXED(b.grid[y][x]) { \
            putchar(BITS_TO_NUM(b.grid[y][x]) + '0'); \
        } else { \
            PRINT_PENCIL(b.grid[y][x]); \
        } \
        putchar(' '); \
        if (x == SIZE - 1) putchar('\n'); \
    ) \
} while (0)

board solve_sudoku(board b) {
    
    FOR_BOARD(row, col, 
        if IS_FIXED(b.grid[row][col]) continue;
        FOR (try_num, 1, 10,
            if (!(NUM_TO_BITS(try_num) & b.grid[row][col])) continue;
            board try_board = b;
            FIX_NUMBER(try_board, row, col, try_num);
            if (try_board.status & INSOLUBLE) continue;
            try_board = solve_sudoku(try_board);
            if (try_board.status & SOLVED) return try_board;
        )
        b.status |= INSOLUBLE;
        return b;
    )
    b.status |= SOLVED;
    return b;

}

int main(int argc, char **argv) {
    
    // 53..7....6..195....98....6.8...6...34..8.3..17...2...6.6....28....419..5....8..79
    // 53..7..../6..195.../.98....6./8...6...3/4..8.3..1/7...2...6/.6....28./...419..5/....8..79
    // 53oo7oooo+6oo195ooo+o98oooo6o+8ooo6ooo3+4oo8o3oo1+7ooo2ooo6+o6oooo28o+ooo419oo5+oooo8oo79

    while (1) {
        puts("Enter a sudoku board (either an 81-char string (no row separators) or an 89-char string).");
        puts("Please ensure that empty square placeholders and line separators (if used) are consistent.");
        fputs("Board: ", stdout);
        char board_string[91];
        fgets(board_string, 91, stdin);
        int len = strlen(board_string) - 1;
        if (len == 1) return 0;
        if (len != 81 && len != 89) {
            puts("Input string is of incorrect length!");
            continue;
        }
        char *ptr = board_string, empty_clue = '\0', line_sep = (len == 89 ? ptr[9] : '\0');
        
        board b = EMPTY_BOARD;
        FOR_BOARD(y, x, 
            if (*ptr >= '1' && *ptr <= '9') {
                FIX_NUMBER(b, y, x, *ptr - '0');
            } else {
                if (empty_clue == '\0') empty_clue = *ptr;
                if (*ptr != empty_clue) {
                    puts("Empty clue character is not consistent!");
                    goto InputError;
                }
            }
            ptr++;
            if (x == SIZE - 1 && y < SIZE - 1 && len == 89) {
                if (*ptr != line_sep) {
                    puts("Line separator is not consistent!");
                    goto InputError;
                }
                ptr++;
            }
        )
        // PRINT_BOARD(b);
        b = solve_sudoku(b);
        if (b.status & SOLVED) PRINT_BOARD(b); else puts("No solution found!");
        puts("");
        continue;

InputError:
        continue;
    }
}