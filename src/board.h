#pragma once

#include <p6/p6.h>

struct CellIndex {
    int x;
    int y;
};

float cell_radius(int board_size);

glm::vec2 cell_bottom_left_corner(CellIndex index, int board_size);

glm::vec2 cell_center(CellIndex index, int board_size);

/// Draws a cell at the position specified by `index`
/// It uses the current context's fill, stroke and stroke_weight
void draw_cell(CellIndex index, int board_size, p6::Context& ctx);

/// Draws a game board
/// size is the number of rows and the number of columns
/// It uses the current context's fill, stroke and stroke_weight
void draw_board(int size, p6::Context& ctx);