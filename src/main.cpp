#include <p6/p6.h>
#include <array>
#include <optional>
#include "menu.h"

struct CellIndex {
    int x;
    int y;
};

float cell_radius(int board_size)
{
    return 1.f / static_cast<float>(board_size);
}

glm::vec2 cell_bottom_left_corner(CellIndex index, int board_size)
{
    const auto idx = glm::vec2{static_cast<float>(index.x),
                               static_cast<float>(index.y)};
    return p6::map(idx,
                   glm::vec2{0.f}, glm::vec2{static_cast<float>(board_size)},
                   glm::vec2{-1.f}, glm::vec2{1.f});
}

glm::vec2 cell_center(CellIndex index, int board_size)
{
    return cell_bottom_left_corner(index, board_size) + cell_radius(board_size);
}

/// Draws a cell at the position specified by `index`
/// It uses the current context's fill, stroke and stroke_weight
void draw_cell(CellIndex index, int board_size, p6::Context& ctx)
{
    ctx.square(p6::BottomLeftCorner{cell_bottom_left_corner(index, board_size)},
               p6::Radius{cell_radius(board_size)});
}

void draw_nought(CellIndex index, int board_size, p6::Context& ctx)
{
    ctx.stroke        = {0, 0, 0};
    ctx.fill          = {0, 0, 0, 0};
    ctx.stroke_weight = 0.4f * cell_radius(board_size);
    ctx.circle(p6::Center{cell_center(index, board_size)},
               p6::Radius{0.9f * cell_radius(board_size)});
}

void draw_cross(CellIndex index, int board_size, p6::Context& ctx)
{
    ctx.stroke          = {0, 0, 0};
    ctx.fill            = {0, 0, 0, 0};
    ctx.stroke_weight   = 0.4f * cell_radius(board_size);
    const auto center   = p6::Center{cell_center(index, board_size)};
    const auto radii    = p6::Radii{glm::vec2{1.f, 0.2f} * cell_radius(board_size)};
    const auto rotation = p6::Rotation{0.125_turn};
    ctx.rectangle(center, radii, rotation);
    ctx.rectangle(center, radii, -rotation);
}

/// Draws a game board
/// size is the number of rows and the number of columns
/// It uses the current context's fill, stroke and stroke_weight
void draw_board(int size, p6::Context& ctx)
{
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            draw_cell({x, y}, size, ctx);
        }
    }
}

enum class Player {
    Noughts,
    Crosses,
};

template<int size>
class Board {
public:
    std::optional<Player>& operator[](CellIndex index)
    {
        assert(index.x >= 0 && index.x < size &&
               index.y >= 0 && index.y < size);
        return _cells[index.x][index.y];
    }

    const std::optional<Player>& operator[](CellIndex index) const
    {
        assert(index.x >= 0 && index.x < size && // Unfortunately I don't think there is a way to avoid this duplication (without using macros).
               index.y >= 0 && index.y < size);  // We need both the const version to use when our Board is const and we just want to read from it
        return _cells[index.x][index.y];         // And also the non-const version to modify the Board
    }

private:
    std::array<std::array<std::optional<Player>, size>, size> _cells;
};

std::optional<CellIndex> cell_hovered_by(glm::vec2 position, int board_size)
{
    const auto pos   = p6::map(position,
                             glm::vec2{-1.f}, glm::vec2{1.f},
                             glm::vec2{0.f}, glm::vec2{static_cast<float>(board_size)});
    const auto index = CellIndex{
        static_cast<int>(std::floor(pos.x)),
        static_cast<int>(std::floor(pos.y))};
    if (index.x >= 0 && index.x < board_size &&
        index.y >= 0 && index.y < board_size) {
        return std::make_optional(index);
    }
    else {
        return std::nullopt;
    }
}

void draw_player(Player player, CellIndex index, int board_size, p6::Context& ctx)
{
    if (player == Player::Noughts) {
        draw_nought(index, board_size, ctx);
    }
    else {
        draw_cross(index, board_size, ctx);
    }
}

template<int size>
void draw_noughts_and_crosses(const Board<size>& board, p6::Context& ctx)
{
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            const auto cell = board[{x, y}];
            if (cell.has_value()) {
                draw_player(*cell, {x, y}, size, ctx);
            }
        }
    }
}

void change_player(Player& player)
{
    if (player == Player::Noughts) {
        player = Player::Crosses;
    }
    else {
        player = Player::Noughts;
    }
}

template<int board_size>
void try_to_play(std::optional<CellIndex> cell_index, Board<board_size>& board, Player& current_player)
{
    if (cell_index.has_value()) {
        const auto cell_is_empty = !board[*cell_index].has_value();
        if (cell_is_empty) {
            board[*cell_index] = current_player;
            change_player(current_player);
        }
    }
}

template<int board_size>
void try_draw_player_on_hovered_cell(Player player, Board<board_size> board, p6::Context& ctx)
{
    const auto hovered_cell = cell_hovered_by(ctx.mouse(), board_size);
    if (hovered_cell.has_value() && !board[*hovered_cell].has_value()) {
        draw_player(player, *hovered_cell, board_size, ctx);
    }
}

int main()
{
    static constexpr int board_size     = 3;
    auto                 board          = Board<board_size>{};
    auto                 current_player = Player::Crosses;
    auto                 ctx            = p6::Context{{800, 800, "Noughts and Crosses"}};

    ctx.mouse_pressed = [&](p6::MouseButton event) {
        try_to_play(cell_hovered_by(event.position, board_size), board, current_player);
    };
    ctx.update = [&]() {
        ctx.background({.3f, 0.25f, 0.35f});
        ctx.stroke_weight = 0.01f;
        ctx.stroke        = {1.f, 1.f, 1.f, 1.f};
        ctx.fill          = {0.f, 0.f, 0.f, 0.f};
        draw_board(board_size, ctx);
        draw_noughts_and_crosses(board, ctx);
        try_draw_player_on_hovered_cell(current_player, board, ctx);
    };
    ctx.start();
}
