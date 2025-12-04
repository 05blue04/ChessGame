#ifndef GUI_H
#define GUI_H

#include "board.h"
#include "move.h"
#include "raylib.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define SQUARE_SIZE (800 / 8)

// Button dimensions
#define RESET_BUTTON_X 820
#define RESET_BUTTON_Y 50
#define RESET_BUTTON_WIDTH 160
#define RESET_BUTTON_HEIGHT 50

// GUI state for managing piece selection
typedef struct {
    int selected;           // Is a piece selected?
    Square selected_square; // Which square is selected
    Board *board;          // Pointer to the chess board
} GUIState;

// Initialize GUI and load assets
void gui_init(void);

// Clean up GUI resources
void gui_cleanup(void);

// Main game loop
void gui_run_game(Board *b);

// Draw the chess board
void gui_draw_board(Board *b, GUIState *state);

// Draw pieces on the board
void gui_draw_pieces(Board *b);

// Convert mouse position to board square
Square gui_get_square_from_mouse(Vector2 mouse_pos);

// Convert board square to screen position
Vector2 gui_get_screen_pos(Square sq);

// Handle mouse input for piece movement
void gui_handle_input(Board *b, GUIState *state);

#endif // GUI_H