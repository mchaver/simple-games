#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// white
// black
// eight pawns
// two rooks
// two bishop
// two knights

typedef enum _team {
  black,
  white,
  no_team, // for empty spaces
} team;

typedef enum _piece {
  pawn,
  rook,
  knight,
  bishop,
  queen,
  king,
  empty,
} piece;

typedef struct _piece_and_team {
  team team;
  piece piece;
} piece_and_team;

void print_piece_and_team(piece_and_team *pt) {
  
  switch (pt->team) {
  case black:
    printf("black ");
    break;
  case white:
    printf("white ");
    break;
  case no_team:
    break;    
  default:
    break;
  }

  switch (pt->piece) {
  case pawn:
    printf("pawn\n");
    break;
  case rook:
    printf("rook\n");
    break;
  case knight:
    printf("knight\n");
    break;
  case bishop:
    printf("bishop\n");
    break;
  case queen:
    printf("queen\n");
    break;
  case king:
    printf("king\n");
    break;
  case empty:
    printf("empty\n");
    break;            
  default:
    break;
  }  
}

typedef struct _board {
  team turn;
  piece_and_team placement[8][8]; // start in upper left corner
} board;

typedef struct _position {
  int col; // 0-7, a-b on chessboard
  int row; // 0-7, 8-1 on chessboard
} position;

// typedef struct _move {
//   struct _move* next_move;
//   int x;
//   int y;
// } move;

// typedef struct _moves {
//   move *move;
// } moves;

typedef struct _moves {
  position *positions;
  int size;
} moves;

void add_move(moves* moves, int row, int col) {
  moves->size++;
  moves->positions = realloc(moves->positions, moves->size * sizeof(position));
  if (!moves->positions) {
    printf("add_move failed to relloc size.\n");
    exit(0);
  }
  moves->positions[moves->size-1].row = row;
  moves->positions[moves->size-1].col = col;
};

char* position_to_code(position *p) {
  char* code = malloc(2 * sizeof(char));

  switch (p->col) {
  case 0:
    code[0] = 'a';
    break;
  case 1:
    code[0] = 'b';
    break;
  case 2:
    code[0] = 'c';
    break;
  case 3:
    code[0] = 'd';
    break;
  case 4:
    code[0] = 'e';
    break;
  case 5:
    code[0] = 'f';
    break;
  case 6:
    code[0] = 'g';
    break;
  case 7:
    code[0] = 'h';
    break;
  default:
    break;
  }
    
  switch (p->row) {
  case 0:
    code[1] = '8';
    break;
  case 1:
    code[1] = '7';
    break;
  case 2:
    code[1] = '6';
    break;
  case 3:
    code[1] = '5';
    break;
  case 4:
    code[1] = '4';
    break;
  case 5:
    code[1] = '3';
    break;
  case 6:
    code[1] = '2';
    break;
  case 7:
    code[1] = '1';
    break;
  default:
    break;
  }

  return code;
}

void print_moves(moves* moves) {
  int i;
  for (i = 0; i < moves->size; i++) {
    char* code = position_to_code(&moves->positions[i]);
    printf("Can move to %s row: %d, col: %d\n", code, moves->positions[i].row, moves->positions[i].col);
    free(code);
  }
};

void free_moves(moves* moves) {
  // int i;
  // for (i = 0; i < moves->size; i++) {
  //   free(&moves->positions[i]);
  // }
  free(moves->positions);
  free(moves);
}

// typedef struct _moves {
//   int count; // total number of moves
//   int* position;
// } moves;

const piece_and_team empty_space = {.team = no_team, .piece = empty};

const piece_and_team black_pawn = {.team = black, .piece = pawn};
const piece_and_team black_rook = {.team = black, .piece = rook};
const piece_and_team black_knight = {.team = black, .piece = knight};
const piece_and_team black_bishop = {.team = black, .piece = bishop};
const piece_and_team black_king = {.team = black, .piece = king};
const piece_and_team black_queen = {.team = black, .piece = queen};

const piece_and_team white_pawn = {.team = white, .piece = pawn};
const piece_and_team white_rook = {.team = white, .piece = rook};
const piece_and_team white_knight = {.team = white, .piece = knight};
const piece_and_team white_bishop = {.team = white, .piece = bishop};
const piece_and_team white_king = {.team = white, .piece = king};
const piece_and_team white_queen = {.team = white, .piece = queen};

board initial_board = {
  .turn = white,
  .placement = {
    {  black_rook, black_knight, black_bishop, black_king, black_queen, black_bishop, black_knight, black_rook
    },
    {  black_pawn, black_pawn, black_pawn, black_pawn, black_pawn, black_pawn, black_pawn, black_pawn
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  white_pawn, white_pawn, white_pawn, white_pawn, white_pawn, white_pawn, white_pawn, white_pawn
    },
    {  white_rook, white_knight, white_bishop, white_king, white_queen, white_bishop, white_knight, white_rook
    },    
  }
};

// pawn, on first move can advance forward one or two spaces
// captures forward,diagonally by one space
// en passant, enemy pawn moves two spaces forward, lands beside your pawn, you move diagonally and capture it

// column, row
void can_move(board *board, int row, int col) {
  moves *moves = malloc(sizeof(moves));
  // collect valid moves
  
  if (row < 0 || row > 7 || col < 0 || col > 7 ) {
    return; // position does not exist
  }

  piece_and_team *p = &board->placement[row][col];
  if (p->piece == empty) {
    return;
  }
  print_piece_and_team(p);
  if (p->piece == pawn) {
    if (p->team == white) {
      if (row == 6) {
        // it is on the beginning row
        if (board->placement[5][col].piece == empty) {
          if (board->placement[4][col].piece == empty) {
            // can move two spaces forward
            add_move(moves, 4, col);
          }
          // can move forward one space
          add_move(moves, 5, col);
        }
      } else {
        if (row > 0 && board->placement[row-1][col].piece == empty) {
          // can move one space forward
          add_move(moves, row-1, col);
        }
      }
      // check if can capture
      if (row > 0) {
        // check left capture
        if (col > 0 && board->placement[row-1][col-1].team == black) {
          // can left capture
          add_move(moves, row-1, col-1);
        }

        // check right capture
        if (row < 7 && board->placement[row-1][col+1].team == black) {
          // can left capture
          add_move(moves, row-1, col+1);
        }        
      }
    } else {
      // black pawn
      if (row == 1) {
        // it is on the beginning row
        if (board->placement[2][col].piece == empty) {
          if (board->placement[3][col].piece == empty) {
            // can move two spaces forward
            add_move(moves, 3, col);
          }
          // can move one space forward
          add_move(moves, 2, col);
        }
      } else {
        if (row < 7 && board->placement[row+1][col].piece == empty) {
          // can move one space forward
          add_move(moves, row+1, col);
        }
      }
      // check if can capture
      if (row < 7 ) {
        // check left capture
        if (col > 0 && board->placement[row+1][col-1].team == white) {
          // can left capture
          add_move(moves, row+1, col-1);
        }

        // check right capture
        if (col < 7 && board->placement[row+1][col+1].team == white) {
          // can right capture
          add_move(moves, row+1, col+1);
        }        
      }      
    }
    print_moves(moves);
    free_moves(moves);
  }
}

int main() {
  // print_piece_and_team(&initial_board.placement[0][0]);
  // print_piece_and_team(&initial_board.placement[0][6]);
  // print_piece_and_team(&initial_board.placement[6][0]);
  can_move(&initial_board, 6, 0);
  can_move(&initial_board, 1, 1);  
  return 0;
}
