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

int are_enemy_teams(piece_and_team *pt1, piece_and_team *pt2) {
  if ((pt1->team == white && pt2->team == black) ||
      (pt1->team == black && pt2->team == white)) {
    return 1;
  }
  return 0;
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

void print_piece_and_team(piece_and_team *pt, int row, int col) {  
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
    printf("pawn");
    break;
  case rook:
    printf("rook");
    break;
  case knight:
    printf("knight");
    break;
  case bishop:
    printf("bishop");
    break;
  case queen:
    printf("queen");
    break;
  case king:
    printf("king");
    break;
  case empty:
    printf("empty");
    break;            
  default:
    break;
  }

  position p;
  p.col = col;
  p.row = row;
  char* code = position_to_code(&p);
  printf(" at %s row: %d, col: %d\n", code, p.row, p.col);
  free(code);
}


void print_moves(moves* moves) {
  int i;
  char* code;
  for (i = 0; i < moves->size; i++) {
    code = position_to_code(&moves->positions[i]);
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

board rook_test_board = {
  .turn = white,
  .placement = {
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, white_rook, empty_space, empty_space, black_queen, empty_space, empty_space, empty_space
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, white_pawn, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },    
  }
};

board bishop_test_board = {
  .turn = black,
  .placement = {
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, white_pawn, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, empty_space, black_bishop, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  white_rook, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },
    {  empty_space, white_pawn, empty_space, empty_space, empty_space, black_queen, empty_space, empty_space
    },
    {  empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space, empty_space
    },    
  }
};

// pawn, on first move can advance forward one or two spaces
// captures forward,diagonally by one space
// en passant, enemy pawn moves two spaces forward, lands beside your pawn, you move diagonally and capture it

const int BLACK_PAWN_START_ROW = 1;
const int WHITE_PAWN_START_ROW = 6;

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
  
  print_piece_and_team(p, row, col);
  
  if (p->piece == pawn) {
    // get start_row and direction for either team
    int start_row = 0;
    int direction = 0;
    if (p->team == white) {
      direction = -1;
      if (row == WHITE_PAWN_START_ROW) start_row = WHITE_PAWN_START_ROW;
    } else if (p->team == black) {
      direction = 1;
      if (row == BLACK_PAWN_START_ROW) start_row = BLACK_PAWN_START_ROW;
    }

    // the pawn is on the start_row, see if it can move two spaces
    int next_row = start_row + direction;
    
    if (start_row != 0) {
      int next_next_row = start_row + direction + direction;
      if (board->placement[next_row][col].piece == empty) {
        if (board->placement[next_next_row][col].piece == empty) {
          // can move two spaces forward
          add_move(moves, next_next_row, col);
        }

      }
    }

    // see if it can move one space
    if (row > 0 && row < 7 && board->placement[next_row][col].piece == empty) {
      // can move forward one space
      add_move(moves, next_row, col);
    }

    // check if the pawn can capture
    if (row > 0 && row < 7) {
      // check left capture
      if (col > 0 && are_enemy_teams(&board->placement[next_row][col-1], p)) {
        // can left capture
        add_move(moves, next_row, col-1);
      }

      // check right capture
      if (row > 0 && row < 7 && are_enemy_teams(&board->placement[next_row][col+1], p)) {
        // can left capture
        add_move(moves, next_row, col+1);
      }        
    }
  } else if (p->piece == rook) {
    int cursor = row + 1;
    // up moves
    while (cursor < 8) {
      if (board->placement[cursor][col].piece == empty) {
        add_move(moves, cursor, col);
        cursor++;
      } else if (are_enemy_teams(&board->placement[cursor][col], p)) {
        add_move(moves, cursor, col);
        break;
      } else {
        // same team, can't make this move
        break;
      }
    }
    cursor = row - 1;
    // down moves
    while (cursor > -1) {
      if (board->placement[cursor][col].piece == empty) {
        add_move(moves, cursor, col);
        cursor--;
      } else if (are_enemy_teams(&board->placement[cursor][col], p)) {
        add_move(moves, cursor, col);
        break;
      } else {
        // same team, can't make this move
        break;
      }
    }  
    // left moves
    cursor = col - 1;
    while (cursor > -1) {
      if (board->placement[row][cursor].piece == empty) {
        add_move(moves, row, cursor);
        cursor--;
      } else if (are_enemy_teams(&board->placement[row][cursor], p)) {
        add_move(moves, row, cursor);
        break;
      } else {
        // same team, can't make this move
        break;
      }
    }
    // right moves
    cursor = col + 1;
    while (cursor < 8) {
      if (board->placement[row][cursor].piece == empty) {
        add_move(moves, row, cursor);
        cursor++;
      } else if (are_enemy_teams(&board->placement[row][cursor], p)) {
        add_move(moves, row, cursor);
        break;
      } else {
        // same team, can't make this move
        break;
      }
    }    
  } else if (p->piece == bishop) {
    int row_cursor = row + 1;
    int col_cursor = col + 1;
    
    // up and right movement
    while (row_cursor < 8 && col_cursor < 8) {
      if (board->placement[row_cursor][col_cursor].piece == empty) {
        add_move(moves, row_cursor, col_cursor);
        row_cursor++;
        col_cursor++;
      } else if (are_enemy_teams(&board->placement[row_cursor][col_cursor], p)) {
        add_move(moves, row_cursor, col_cursor);
        break;
      } else {
        // same team, can't make this move
        break;
      }
    }

    // up and left movement    
    row_cursor = row + 1;
    col_cursor = col - 1;
    while (row_cursor < 8 && col_cursor > -1) {
      if (board->placement[row_cursor][col_cursor].piece == empty) {
        add_move(moves, row_cursor, col_cursor);
        row_cursor++;
        col_cursor--;
      } else if (are_enemy_teams(&board->placement[row_cursor][col_cursor], p)) {
        add_move(moves, row_cursor, col_cursor);
        break;
      } else {
        // same team, can't make this move
        break;
      }
    }

    // down and right movement    
    row_cursor = row - 1;
    col_cursor = col + 1;
    while (row_cursor > -1 && col_cursor < 8) {
      if (board->placement[row_cursor][col_cursor].piece == empty) {
        add_move(moves, row_cursor, col_cursor);
        row_cursor--;
        col_cursor++;
      } else if (are_enemy_teams(&board->placement[row_cursor][col_cursor], p)) {
        add_move(moves, row_cursor, col_cursor);
        break;
      } else {
        // same team, can't make this move
        break;
      }
    }

    // down and left movement    
    row_cursor = row - 1;
    col_cursor = col - 1;
    while (row_cursor > -1 && col_cursor > -1) {
      if (board->placement[row_cursor][col_cursor].piece == empty) {
        add_move(moves, row_cursor, col_cursor);
        row_cursor--;
        col_cursor--;
      } else if (are_enemy_teams(&board->placement[row_cursor][col_cursor], p)) {
        add_move(moves, row_cursor, col_cursor);
        break;
      } else {
        // same team, can't make this move
        break;
      }
    }
  }

  print_moves(moves);
  free_moves(moves);  
}

int main() {
  // print_piece_and_team(&initial_board.placement[0][0]);
  // print_piece_and_team(&initial_board.placement[0][6]);
  // print_piece_and_team(&initial_board.placement[6][0]);
  
  can_move(&initial_board, 6, 0);
  can_move(&initial_board, 1, 1);
  can_move(&rook_test_board, 4, 1);
  can_move(&bishop_test_board, 3, 2);

  return 0;
}
