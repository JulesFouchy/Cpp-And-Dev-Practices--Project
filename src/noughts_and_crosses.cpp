#include "noughts_and_crosses.h"
#include <p6/p6.h>
#include <algorithm>
#include <iostream>
#include "board.h"

enum class Player {
    Noughts,
    Crosses,
};

using Board = BoardT<3, 3, Player>;

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

void draw_noughts_and_crosses(const Board& board, p6::Context& ctx)
{
    for (int x = 0; x < board.width(); ++x) {
        for (int y = 0; y < board.height(); ++y) {
            const auto cell = board[{x, y}];
            if (cell.has_value()) {
                draw_player(*cell, {x, y}, board.height(), ctx);
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

void try_to_play(std::optional<CellIndex> cell_index, Board& board, Player& current_player)
{
    if (cell_index.has_value()) {
        const auto cell_is_empty = !board[*cell_index].has_value();
        if (cell_is_empty) {
            board[*cell_index] = current_player;
            change_player(current_player);
        }
    }
}

void try_draw_player_on_hovered_cell(Player player, Board board, p6::Context& ctx)
{
    const auto hovered_cell = cell_hovered_by(ctx.mouse(), board.height());
    if (hovered_cell.has_value() && !board[*hovered_cell].has_value()) {
        draw_player(player, *hovered_cell, board.height(), ctx);
    }
}

bool board_is_full(const Board& board)
{
    return std::all_of(board.begin(), board.end(), [](const auto& cell) {
        return cell.has_value();
    });
}

std::optional<Player> check_for_winner_on_line(const Board& board, std::function<CellIndex(int)> index_generator)
{
    const bool are_all_equal = [&]() {
        for (int position = 0; position < board.height() - 1; ++position) {
            if (board[index_generator(position)] != board[index_generator(position + 1)]) {
                return false;
            }
        }
        return true;
    }();
    if (are_all_equal && board[index_generator(0)].has_value()) {
        return *board[index_generator(0)];
    }
    else {
        return std::nullopt;
    }
}

std::optional<Player> check_for_winner(const Board& board)
{
    std::optional<Player> winner = std::nullopt;
    // Columns
    for (int x = 0; x < board.width() && !winner.has_value(); ++x) {
        winner = check_for_winner_on_line(board, [x](int position) {
            return CellIndex{x, position};
        });
    }
    // Rows
    for (int y = 0; y < board.height() && !winner.has_value(); ++y) {
        winner = check_for_winner_on_line(board, [y](int position) {
            return CellIndex{position, y};
        });
    }
    // Diagonal
    if (!winner.has_value()) {
        winner = check_for_winner_on_line(board, [](int position) {
            return CellIndex{position, position};
        });
    }
    // Anti-diagonal
    if (!winner.has_value()) {
        winner = check_for_winner_on_line(board, [&](int position) {
            return CellIndex{position, board.height() - position - 1};
        });
    }
    return winner;
}

bool game_is_finished(const Board& board)
{
    if (const auto winner = check_for_winner(board); winner.has_value()) {
        if (*winner == Player::Noughts) {
            std::cout << "Noughts have won!\n";
        }
        else {
            std::cout << "Crosses have won!\n";
        }
        return true;
    }
    else if (board_is_full(board)) {
        std::cout << "This is a draw!\n";
        return true;
    }
    else {
        return false;
    }
}

void play_noughts_and_crosses()
{
    auto board          = Board{};
    auto current_player = Player::Crosses;
    auto ctx            = p6::Context{{800, 800, "Noughts and Crosses"}};

    ctx.mouse_pressed = [&](p6::MouseButton event) {
        try_to_play(cell_hovered_by(event.position, board.height()), board, current_player);
    };
    ctx.update = [&]() {
        ctx.background({.3f, 0.25f, 0.35f});
        ctx.stroke_weight = 0.01f;
        ctx.stroke        = {1.f, 1.f, 1.f, 1.f};
        ctx.fill          = {0.f, 0.f, 0.f, 0.f};
        draw_board(board.height(), ctx);
        draw_noughts_and_crosses(board, ctx);
        try_draw_player_on_hovered_cell(current_player, board, ctx);
        if (game_is_finished(board)) {
            ctx.stop();
        }
    };
    ctx.start();
}