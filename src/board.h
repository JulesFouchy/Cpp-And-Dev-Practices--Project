#pragma once

#include <p6/p6.h>

struct CellIndex {
    int x;
    int y;
};

struct BoardSize {
    int width;
    int height;

    BoardSize(int size)
        : width{size}, height{size}
    {
    }

    BoardSize(int width, int height)
        : width{width}, height{height}
    {
    }
};

float cell_radius(BoardSize board_size);

glm::vec2 cell_bottom_left_corner(CellIndex index, BoardSize board_size);

glm::vec2 cell_center(CellIndex index, BoardSize board_size);

/// Draws a cell at the position specified by `index`
/// It uses the current context's fill, stroke and stroke_weight
void draw_cell(CellIndex index, BoardSize board_size, p6::Context& ctx);

/// Draws a game board
/// size is the number of rows and the number of columns
/// It uses the current context's fill, stroke and stroke_weight
void draw_board(BoardSize size, p6::Context& ctx);