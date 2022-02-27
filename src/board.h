#pragma once

#include <p6/p6.h>
#include <array>
#include <optional>

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

template<int board_width, int board_height, typename Player>
class BoardT {
public:
    BoardSize size() const { return {board_width, board_height}; }
    int       width() const { return board_width; }
    int       height() const { return board_height; }

    std::optional<Player>& operator[](CellIndex index)
    {
        assert(index.x >= 0 && index.x < width() &&
               index.y >= 0 && index.y < height());
        return _cells[index.x + index.y * width()];
    }

    const std::optional<Player>& operator[](CellIndex index) const
    {
        assert(index.x >= 0 && index.x < width() && // Unfortunately I don't think there is a way to avoid this duplication (without using macros).
               index.y >= 0 && index.y < height()); // We need both the const version to use when our Board is const and we just want to read from it
        return _cells[index.x + index.y * width()]; // And also the non-const version to modify the Board
    }

    auto begin() { return _cells.begin(); }
    auto begin() const { return _cells.begin(); }
    auto end() { return _cells.end(); }
    auto end() const { return _cells.end(); }

private:
    std::array<std::optional<Player>, board_width * board_height> _cells;
};