#include "board.h"

float aspect_ratio(BoardSize board_size)
{
    return static_cast<float>(board_size.width) /
           static_cast<float>(board_size.height);
}

float cell_radius(BoardSize board_size)
{
    return 1.f / static_cast<float>(board_size.height);
}

glm::vec2 cell_bottom_left_corner(CellIndex index, BoardSize board_size)
{
    const auto  idx   = glm::vec2{static_cast<float>(index.x),
                               static_cast<float>(index.y)};
    const float ratio = aspect_ratio(board_size);
    return p6::map(idx,
                   glm::vec2{0.f}, glm::vec2{static_cast<float>(board_size.width), static_cast<float>(board_size.height)},
                   glm::vec2{-ratio, -1.f}, glm::vec2{ratio, 1.f});
}

glm::vec2 cell_center(CellIndex index, BoardSize board_size)
{
    return cell_bottom_left_corner(index, board_size) + cell_radius(board_size);
}

void draw_cell(CellIndex index, BoardSize board_size, p6::Context& ctx)
{
    ctx.square(p6::BottomLeftCorner{cell_bottom_left_corner(index, board_size)},
               p6::Radius{cell_radius(board_size)});
}

void draw_board(BoardSize size, p6::Context& ctx)
{
    for (int x = 0; x < size.width; ++x) {
        for (int y = 0; y < size.height; ++y) {
            draw_cell({x, y}, size, ctx);
        }
    }
}