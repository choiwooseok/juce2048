#ifndef HPP_2048
#define HPP_2048

#include <string>

constexpr int LINE_SIZE = 4;
constexpr int BOARD_SIZE = LINE_SIZE * LINE_SIZE;
constexpr int END_GAME_TARGET = 2048;

constexpr long COLOR_BOARD = 0xFFBBADA0;
constexpr long COLOR_TEXT = 0xFF776E65;
constexpr long COLOR_TEXT_LIGHT = 0xFFF9F6F2;
constexpr long COLOR_TILE_0 = 0xFFCDC1B4;
constexpr long COLOR_TILE_2 = 0xFFEEE4DA;
constexpr long COLOR_TILE_4 = 0xFFEDE0C8;
constexpr long COLOR_TILE_8 = 0xFFF2B179;
constexpr long COLOR_TILE_16 = 0xFFF59563;
constexpr long COLOR_TILE_32 = 0xFFF67C5F;
constexpr long COLOR_TILE_64 = 0xFFF65E3B;
constexpr long COLOR_TILE_128 = 0xFFEDCF72;
constexpr long COLOR_TILE_256 = 0xFFEDCC61;
constexpr long COLOR_TILE_512 = 0xFFEDC850;
constexpr long COLOR_TILE_1024 = 0xFFEDC53F;
constexpr long COLOR_TILE_2048 = 0xFFEDC22E;

#define E_RANDOM ((unsigned int)rand() % 100 + 1)
#define E_TILE_AT(x, y) (e_board[(x) + ((y)*LINE_SIZE)])

class Game {
  int e_win;
  int e_lose;
  int e_score;
  int e_board[BOARD_SIZE];

  int *f_space[BOARD_SIZE];
  int b_reg[LINE_SIZE];
  int f_reg[LINE_SIZE];

  int K_ESCAPE;
  int K_LEFT;
  int K_RIGHT;
  int K_UP;
  int K_DOWN;

  int *GetLine(int index, int *reg) {
    for (int i = 0; i < LINE_SIZE; ++i) {
      reg[i] = E_TILE_AT(i, index);
    }
    return reg;
  }

  void SetLine(int index, int *line) {
    for (int i = 0; i < LINE_SIZE; ++i) {
      e_board[index * LINE_SIZE + i] = line[i];
    }
  }

  int *MoveLine(int *line) {
    int size = 0;
    for (int i = 0; i < LINE_SIZE; ++i) {
      if (line[i]) {
        f_reg[size++] = line[i];
      }
    }

    if (!size) {
      memcpy(f_reg, line, LINE_SIZE * sizeof(int));
    }
    return f_reg;
  }

  int *MergeLine(int *line) {
    int size = 0;
    ResetRegB();
    for (int i = 0; i < LINE_SIZE && line[i]; ++i) {
      int value = line[i];
      if (i < (LINE_SIZE - 1) && line[i] == line[i + 1]) {
        value *= 2;
        e_score += value;
        if (value == END_GAME_TARGET) {
          e_win = 1;
        }
        ++i;
      }
      b_reg[size++] = value;
    }

    if (!size) {
      memcpy(b_reg, line, LINE_SIZE * sizeof(int));
    }
    return b_reg;
  }

  int CompareLine(int *line_first, int *line_second) {
    for (int i = 0; i < LINE_SIZE; ++i) {
      if (line_first[i] != line_second[i]) {
        return 0;
      }
    }
    return 1;
  }

  int CanMove() {
    if (UpdateSpace()) {
      return 1;
    }

    for (int x = 0; x < LINE_SIZE; ++x) {
      for (int y = 0; y < LINE_SIZE; ++y) {
        if ((x < (LINE_SIZE - 1) && E_TILE_AT(x, y) == E_TILE_AT(x + 1, y)) ||
            (y < (LINE_SIZE - 1) && E_TILE_AT(x, y) == E_TILE_AT(x, y + 1))) {
          return 1;
        }
      }
    }
    return 0;
  }

  void Reset() {
    e_score = e_win = e_lose = 0;
    memset(e_board, 0, BOARD_SIZE * sizeof(int));
    ResetSpace();
    ResetRegs();
    AddTile(2);
  }

  void ResetRegB() {
    memset(b_reg, 0, LINE_SIZE * sizeof(int));
  }

  void ResetRegs() {
    memset(b_reg, 0, LINE_SIZE * sizeof(int));
    memset(f_reg, 0, LINE_SIZE * sizeof(int));
  }

  void ResetSpace() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
      f_space[i] = NULL;
    }
  }

  int UpdateSpace() {
    int size = 0;
    ResetSpace();
    for (int i = 0; i < BOARD_SIZE; ++i) {
      if (!e_board[i]) {
        f_space[size++] = &e_board[i];
      }
    }
    return size;
  }

  void AddTile(int n) {
    for (int i = 0; i < n; ++i) {
      const int size = UpdateSpace();
      if (size) {
        *f_space[(E_RANDOM * size / 100) % size] = (E_RANDOM < 90) ? 2 : 4;
      }
    }
  }

  void Rotate(int a) {
    for (; a >= 90; a -= 90) {
      int rotated[BOARD_SIZE];
      for (int x = 0; x < LINE_SIZE; ++x) {
        for (int y = 0; y < LINE_SIZE; ++y) {
          rotated[x * LINE_SIZE + y] = e_board[(LINE_SIZE - y - 1) * LINE_SIZE + x];
        }
      }
      memcpy(e_board, rotated, BOARD_SIZE * sizeof(int));
    }
  }

  void Left() {
    int *line, *merged, need_add_tile = 0;
    for (int i = 0; i < LINE_SIZE; ++i) {
      ResetRegs();
      line = GetLine(i, b_reg);
      merged = MergeLine(MoveLine(line));
      line = GetLine(i, f_reg);
      int res = CompareLine(line, merged);
      SetLine(i, merged);
      if (!need_add_tile && !res) {
        need_add_tile = 1;
      }
    }

    if (need_add_tile) {
      AddTile(1);
    }
  }

  void Right() {
    Rotate(180);
    Left();
    Rotate(180);
  }

  void Up() {
    Rotate(270);
    Left();
    Rotate(90);
  }

  void Down() {
    Rotate(90);
    Left();
    Rotate(270);
  }

 public:
  Game() {
    Reset();
  }

  Game(int esc_keycode, int left_keycode, int right_keycode, int up_keycode, int down_keycode) {
    K_ESCAPE = esc_keycode;
    K_LEFT = left_keycode;
    K_RIGHT = right_keycode;
    K_UP = up_keycode;
    K_DOWN = down_keycode;
    Reset();
  }

  void KeyPressed(int keycode) {
    if (keycode == K_ESCAPE) {
      Reset();
    }

    if (!CanMove()) {
      e_lose = 1;
    }

    if (!e_win && !e_lose) {
      if (keycode == K_LEFT) {
        Left();
      } else if (keycode == K_RIGHT) {
        Right();
      } else if (keycode == K_UP) {
        Up();
      } else if (keycode == K_DOWN) {
        Down();
      }
    }

    if (!e_win && !CanMove()) {
      e_lose = 1;
    }
  }

  unsigned long Foreground(int value) {
    return (value < 16) ? COLOR_TEXT : COLOR_TEXT_LIGHT;
  }

  unsigned long Background(int value) {
    switch (value) {
      case 2:
        return COLOR_TILE_2;
      case 4:
        return COLOR_TILE_4;
      case 8:
        return COLOR_TILE_8;
      case 16:
        return COLOR_TILE_16;
      case 32:
        return COLOR_TILE_32;
      case 64:
        return COLOR_TILE_64;
      case 128:
        return COLOR_TILE_128;
      case 256:
        return COLOR_TILE_256;
      case 512:
        return COLOR_TILE_512;
      case 1024:
        return COLOR_TILE_1024;
      case 2048:
        return COLOR_TILE_2048;
    }
    return COLOR_TILE_0;
  }

  int Board(int idx) {
    return e_board[idx];
  }

  int Score() {
    return e_score;
  }

  int IsEnd() {
    return e_win || e_lose;
  }

  std::string GetGameOverMsg() {
    return e_win ? "You Won!" : "Game Over!";
  }
};
#endif  // HPP_2048