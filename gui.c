#include "gui.h"
#include <stdio.h>

// Piece textures
static Texture2D piece_textures[13]; // EMPTY + 12 pieces


// Load all piece images
void gui_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Chess Engine");
    SetTargetFPS(60);
    
    // Load piece textures
    piece_textures[wPawn]   = LoadTexture("assets/wp.png");
    piece_textures[wKnight] = LoadTexture("assets/wn.png");
    piece_textures[wBishop] = LoadTexture("assets/wb.png");
    piece_textures[wRook]   = LoadTexture("assets/wr.png");
    piece_textures[wQueen]  = LoadTexture("assets/wq.png");
    piece_textures[wKing]   = LoadTexture("assets/wk.png");
    
    piece_textures[bPawn]   = LoadTexture("assets/bp.png");
    piece_textures[bKnight] = LoadTexture("assets/bn.png");
    piece_textures[bBishop] = LoadTexture("assets/bb.png");
    piece_textures[bRook]   = LoadTexture("assets/br.png");
    piece_textures[bQueen]  = LoadTexture("assets/bq.png");
    piece_textures[bKing]   = LoadTexture("assets/bk.png");
}

void gui_cleanup(void) {
    // Unload all textures
    for (int i = 1; i <= bKing; i++) {
        UnloadTexture(piece_textures[i]);
    }
    CloseWindow();
}

// Convert board square (21-98) to screen coordinates
Vector2 gui_get_screen_pos(Square sq) {
    int rank = sq / 10 - 2;  // Convert to 0-7
    int file = sq % 10 - 1;  // Convert to 0-7
    
    return (Vector2){
        file * SQUARE_SIZE,
        rank * SQUARE_SIZE
    };
}

// Convert mouse position to board square (21-98)
Square gui_get_square_from_mouse(Vector2 mouse_pos) {
    int file = (int)(mouse_pos.x / SQUARE_SIZE);
    int rank = (int)(mouse_pos.y / SQUARE_SIZE);
    
    // Bounds check
    if (file < 0 || file > 7 || rank < 0 || rank > 7) {
        return -1; // Invalid square
    }
    
    // Convert to 10x12 representation
    return (rank + 2) * 10 + (file + 1);
}

// Check if mouse is over the reset button
static int is_mouse_over_button(Vector2 mouse_pos) {
    return (mouse_pos.x >= RESET_BUTTON_X && 
            mouse_pos.x <= RESET_BUTTON_X + RESET_BUTTON_WIDTH &&
            mouse_pos.y >= RESET_BUTTON_Y && 
            mouse_pos.y <= RESET_BUTTON_Y + RESET_BUTTON_HEIGHT);
}

// Draw the reset button
static void gui_draw_reset_button(void) {
    Vector2 mouse_pos = GetMousePosition();
    int hover = is_mouse_over_button(mouse_pos);
    
    // Button background - lighter when hovered
    Color button_color = hover ? (Color){100, 180, 100, 255} : (Color){70, 140, 70, 255};
    Color border_color = (Color){40, 80, 40, 255};
    
    // Draw button with border
    DrawRectangle(RESET_BUTTON_X, RESET_BUTTON_Y, RESET_BUTTON_WIDTH, RESET_BUTTON_HEIGHT, button_color);
    DrawRectangleLines(RESET_BUTTON_X, RESET_BUTTON_Y, RESET_BUTTON_WIDTH, RESET_BUTTON_HEIGHT, border_color);
    
    // Draw button text centered
    const char *text = "Reset Game";
    int font_size = 20;
    int text_width = MeasureText(text, font_size);
    int text_x = RESET_BUTTON_X + (RESET_BUTTON_WIDTH - text_width) / 2;
    int text_y = RESET_BUTTON_Y + (RESET_BUTTON_HEIGHT - font_size) / 2;
    
    DrawText(text, text_x, text_y, font_size, WHITE);
}

void gui_draw_board(Board *b, GUIState *state) {
    // Draw squares
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Color color;
            
            // Checkerboard pattern
            if ((rank + file) % 2 == 0) {
                color = (Color){240, 217, 181, 255}; // Light square
            } else {
                color = (Color){181, 136, 99, 255};  // Dark square
            }
            
            // Highlight selected square
            Square sq = (rank + 2) * 10 + (file + 1);
            if (state->selected && sq == state->selected_square) {
                color = (Color){255, 255, 0, 150}; // Yellow highlight
            }
            
            // Highlight king square if in check (red tint)
            if ((sq == b->white_king_sq && is_in_check(b, white)) ||
                (sq == b->black_king_sq && is_in_check(b, black))) {
                color = (Color){255, 80, 80, 255}; // Red highlight
            }
            
            DrawRectangle(
                file * SQUARE_SIZE,
                rank * SQUARE_SIZE,
                SQUARE_SIZE,
                SQUARE_SIZE,
                color
            );
        }
    }
    
    // Draw rank/file labels
    const char *files = "abcdefgh";
    for (int i = 0; i < 8; i++) {
        DrawText(TextFormat("%c", files[i]), 
                 i * SQUARE_SIZE + SQUARE_SIZE / 2 - 5, 
                 WINDOW_HEIGHT - 20, 
                 20, 
                 BLACK);
        
        DrawText(TextFormat("%d", 8 - i), 
                 5, 
                 i * SQUARE_SIZE + SQUARE_SIZE / 2 - 10, 
                 20, 
                 BLACK);
    }
    
    // Draw the reset button
    gui_draw_reset_button();
    
    // Draw turn indicator
    const char *turn_text = (b->turn == white) ? "White to move" : "Black to move";
    DrawText(turn_text, RESET_BUTTON_X, RESET_BUTTON_Y + 70, 20, BLACK);
    
    // Draw check indicator
    if (is_in_check(b, b->turn)) {
        DrawText("CHECK!", RESET_BUTTON_X, RESET_BUTTON_Y + 100, 24, RED);
    }

    // Draw check/checkmate/stalemate indicator
    if (is_checkmate(b, b->turn)) {
        const char *winner = (b->turn == white) ? "Black wins!" : "White wins!";
        DrawText("CHECKMATE!", RESET_BUTTON_X, RESET_BUTTON_Y + 100, 24, RED);
        DrawText(winner, RESET_BUTTON_X, RESET_BUTTON_Y + 130, 20, RED);
    } else if (is_stalemate(b, b->turn)) {
        DrawText("STALEMATE!", RESET_BUTTON_X, RESET_BUTTON_Y + 100, 24, ORANGE);
        DrawText("Draw!", RESET_BUTTON_X, RESET_BUTTON_Y + 130, 20, ORANGE);
    } else if (is_in_check(b, b->turn)) {
        DrawText("CHECK!", RESET_BUTTON_X, RESET_BUTTON_Y + 100, 24, RED);
    }
}

void gui_draw_pieces(Board *b) {
    for (int rank = 2; rank <= 9; rank++) {
        for (int file = 1; file <= 8; file++) {
            Square sq = rank * 10 + file;
            Piece piece = b->board[sq];
            
            if (piece != EMPTY) {
                Vector2 pos = gui_get_screen_pos(sq);
                
                int offset = (SQUARE_SIZE - piece_textures[piece].width) / 2;
                DrawTexture(
                    piece_textures[piece],
                    pos.x + offset,
                    pos.y + offset,
                    WHITE
                );
            }
        }
    }
}

void gui_handle_input(Board *b, GUIState *state) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse_pos = GetMousePosition();
        
        // Check if reset button was clicked
        if (is_mouse_over_button(mouse_pos)) {
            reset_board_default(b);
            state->selected = 0;
            state->selected_square = 0;
            printf("Game reset!\n");
            return;
        }
        
        Square clicked_square = gui_get_square_from_mouse(mouse_pos);
        
        if ((int)clicked_square == -1) return; // Invalid square
        
        if (!state->selected) {
            // First click - select piece
            Piece piece = b->board[clicked_square];
            
            if (piece != EMPTY) {
                if ((b->turn == white && piece <= wKing) ||
                    (b->turn == black && piece > wKing)) {
                    state->selected = 1;
                    state->selected_square = clicked_square;
                    printf("Selected square %d\n", clicked_square);
                }
            }
        } else {
            // Second click - try to move
            printf("Attempting move from %d to %d\n", 
                   state->selected_square, clicked_square);
            
            if (is_legal_move(b, state->selected_square, clicked_square)) {
                make_move(b, state->selected_square, clicked_square);
                printf("Move made!\n");
            } else {
                printf("Illegal move!\n");
            }
            
            state->selected = 0;
        }
    }
}

void gui_run_game(Board *b) {
    GUIState state = {0};
    state.board = b;
    
    while (!WindowShouldClose()) {
        gui_handle_input(b, &state);
        
        BeginDrawing();
        ClearBackground(GRAY);
        
        gui_draw_board(b, &state);
        gui_draw_pieces(b);
        
        EndDrawing();
    }
}