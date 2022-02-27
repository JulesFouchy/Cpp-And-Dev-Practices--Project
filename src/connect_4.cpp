#include "connect_4.h"
#include <p6/p6.h>
#include <functional>
#include <iostream>
#include "board.h"

enum class Player {
    Red,
    Yellow,
};

using Board = BoardT<7, 6, Player>;

void draw_token(CellIndex index, BoardSize board_size, Player player, p6::Context& ctx, bool is_preview = false)
{
    const float alpha = is_preview ? 0.25f : 1.f;
    if (player == Player::Red) {
        ctx.fill = {1.f, 0.f, 0.f, alpha};
    }
    else {
        ctx.fill = {1.f, 1.f, 0.f, alpha};
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

std::optional<int> try_to_find_lowest_empty_row_index(int column_index, const Board& board)
{
    for (int y = 0; y < board.height(); ++y) {
        if (!board[{column_index, y}].has_value()) {
            return std::make_optional(y);
        }
    }
    return std::nullopt;
}

/// Returns true iff the column was not full and a token has been successfully added to the column.
bool try_to_play_in_column(int column_index, Player player, Board& board)
{
    const auto row_index = try_to_find_lowest_empty_row_index(column_index, board);
    if (row_index.has_value()) {
        board[{column_index, *row_index}] = std::make_optional(player);
        return true;
    }
    else {
        return false;
    }
}

std::optional<int> column_at(glm::vec2 pos_in_window_space, BoardSize board_size)
{
    const auto x     = pos_in_window_space.x;
    const auto ratio = aspect_ratio(board_size);
    if (-ratio < x && x < ratio) {
        return std::make_optional(
            static_cast<int>(p6::map(x,
                                     -ratio, ratio,
                                     0.f, static_cast<float>(board_size.width))));
    }
    else {
        return std::nullopt;
    }
}

Player next_player(Player player)
{
    if (player == Player::Red) {
        return Player::Yellow;
    }
    else {
        return Player::Red;
    }
}

void preview_token_at(glm::vec2 pos_in_window_space, const Board& board, Player player, p6::Context& ctx)
{
    const auto hovered_column = column_at(pos_in_window_space, board.size());
    if (hovered_column.has_value()) {
        const auto row = try_to_find_lowest_empty_row_index(*hovered_column, board);
        if (row.has_value()) {
            draw_token({*hovered_column, *row},
                       board.size(), player, ctx, true);
        }
    }
}

std::optional<Player> winner_on_line(const Board& board, std::function<std::optional<CellIndex>(int)> index_generator)
{
    auto prev_cell          = index_generator(0);
    auto cell               = index_generator(1);
    int  index              = 1;
    int  consecutive_tokens = 1;
    while (cell.has_value()) {
        if (board[*prev_cell] == board[*cell] && board[*cell].has_value()) {
            consecutive_tokens++;
            if (consecutive_tokens == 4) {
                return *board[*cell];
            }
        }
        else {
            consecutive_tokens = 1;
        }
        index++;
        prev_cell = cell;
        cell      = index_generator(index);
    }
    return std::nullopt;
}

std::optional<Player> winner_on_row(int row_index, const Board& board)
{
    return winner_on_line(board, [&](int index) -> std::optional<CellIndex> {
        if (0 <= index && index < board.width()) {
            return std::make_optional(CellIndex{index, row_index});
        }
        else {
            return std::nullopt;
        }
    });
}

std::optional<Player> winner_on_column(int column_index, const Board& board)
{
    return winner_on_line(board, [&](int index) -> std::optional<CellIndex> {
        if (0 <= index && index < board.height()) {
            return std::make_optional(CellIndex{column_index, index});
        }
        else {
            return std::nullopt;
        }
    });
}

std::optional<Player> winner_on_diagonal(int diagonal_index, const Board& board)
{
    const auto first_cell = diagonal_index < board.height() ? CellIndex{0, board.height() - 1 - diagonal_index}
                                                            : CellIndex{diagonal_index - board.height(), 0};
    return winner_on_line(board, [&](int index) -> std::optional<CellIndex> {
        const auto cell = CellIndex{first_cell.x + index, first_cell.y + index};
        if (cell.x < board.width() && cell.y < board.height()) {
            return std::make_optional(cell);
        }
        else {
            return std::nullopt;
        }
    });
}

std::optional<Player> winner_on_anti_diagonal(int diagonal_index, const Board& board)
{
    const auto first_cell = diagonal_index < board.height() ? CellIndex{0, diagonal_index}
                                                            : CellIndex{diagonal_index - board.height(), board.height() - 1};
    return winner_on_line(board, [&](int index) -> std::optional<CellIndex> {
        const auto cell = CellIndex{first_cell.x + index, first_cell.y - index};
        if (cell.x < board.width() && cell.y >= 0) {
            return std::make_optional(cell);
        }
        else {
            return std::nullopt;
        }
    });
}

std::optional<Player> winner_on_rows(const Board& board)
{
    for (int i = 0; i < board.height(); ++i) {
        const auto winner = winner_on_row(i, board);
        if (winner.has_value()) {
            return winner;
        }
    }
    return std::nullopt;
}

std::optional<Player> winner_on_columns(const Board& board)
{
    for (int i = 0; i < board.width(); ++i) {
        const auto winner = winner_on_column(i, board);
        if (winner.has_value()) {
            return winner;
        }
    }
    return std::nullopt;
}

std::optional<Player> winner_on_diagonals(const Board& board)
{
    for (int i = 0; i < board.width() + board.height() - 1; ++i) {
        const auto winner = winner_on_diagonal(i, board);
        if (winner.has_value()) {
            return winner;
        }
    }
    return std::nullopt;
}

std::optional<Player> winner_on_anti_diagonals(const Board& board)
{
    for (int i = 0; i < board.width() + board.height() - 1; ++i) {
        const auto winner = winner_on_anti_diagonal(i, board);
        if (winner.has_value()) {
            return winner;
        }
    }
    return std::nullopt;
}

std::optional<Player> check_for_winner(const Board& board)
{
    auto winner = winner_on_rows(board);
    if (!winner.has_value()) {
        winner = winner_on_columns(board);
    }
    if (!winner.has_value()) {
        winner = winner_on_diagonals(board);
    }
    if (!winner.has_value()) {
        winner = winner_on_anti_diagonals(board);
    }
    return winner;
}

const char* to_string(Player player)
{
    if (player == Player::Red) {
        return "Red";
    }
    else {
        return "Yellow";
    }
}

bool game_is_over(const Board& board)
{
    if (board_is_full(board)) {
        std::cout << "This is a draw!\n";
        return true;
    }
    else {
        const auto winner = check_for_winner(board);
        if (winner.has_value()) {
            std::cout << to_string(*winner) << " has won!\n";
        }
        return winner.has_value();
    }
}

void play_connect_4()
{
    auto ctx            = p6::Context{{1200, 800, "Connect 4"}};
    auto board          = Board{};
    auto current_player = Player::Red;
    ctx.mouse_pressed   = [&](auto) {
        const auto column_index = column_at(ctx.mouse(), board.size());
        if (column_index.has_value()) {
            if (try_to_play_in_column(*column_index, current_player, board)) {
                current_player = next_player(current_player);
            }
        }
    };
    ctx.update = [&]() {
        ctx.background({.3f, 0.25f, 0.35f});
        ctx.fill = {1.f, 1.f, 1.f, 0.95f};
        draw_board(board.size(), ctx);
        draw_tokens(board, ctx);
        preview_token_at(ctx.mouse(), board, current_player, ctx);
        if (game_is_over(board)) {
            ctx.stop();
        }
    };
    ctx.start();
}