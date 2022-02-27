#include "connect_4.h"
#include <p6/p6.h>
#include "board.h"

enum class Player {
    Red,
    Yellow,
};

using Board = BoardT<7, 6, Player>;

void draw_token(CellIndex index, BoardSize board_size, Player player, p6::Context& ctx)
{
    if (player == Player::Red) {
        ctx.fill = {1.f, 0.f, 0.f};
    }
    else {
        ctx.fill = {1.f, 1.f, 0.f};
    }
    ctx.circle(p6::Center{cell_center(index, board_size)},
               p6::Radius{cell_radius(board_size)});
}

void draw_tokens(const Board& board, p6::Context& ctx)
{
    for (int x = 0; x < board.width(); ++x) {
        for (int y = 0; y < board.height(); ++y) {
            const auto cell = board[{x, y}];
            if (cell.has_value()) {
                draw_token({x, y}, board.size(), *cell, ctx);
            }
        }
    }
}

void play_connect_4()
{
    auto ctx      = p6::Context{{1200, 800, "Connect 4"}};
    auto board    = Board{};
    board[{0, 3}] = std::make_optional(Player::Red);
    board[{1, 2}] = std::make_optional(Player::Yellow);
    ctx.update    = [&]() {
        ctx.background({.3f, 0.25f, 0.35f});
        ctx.fill = {1.f, 1.f, 1.f, 0.5f};
        draw_board(board.size(), ctx);
        draw_tokens(board, ctx);
    };
    ctx.start();
}