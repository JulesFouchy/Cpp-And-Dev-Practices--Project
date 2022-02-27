#include "connect_4.h"
#include <p6/p6.h>
#include "board.h"

void play_connect_4()
{
    auto ctx   = p6::Context{{1200, 800, "Connect 4"}};
    ctx.update = [&]() {
        ctx.background({.3f, 0.25f, 0.35f});
        draw_board({7, 6}, ctx);
    };
    ctx.start();
}