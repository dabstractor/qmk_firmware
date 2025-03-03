#include "./qmk-vim/src/vim.h"
#include "./qmk-vim/src/modes.h"
#include <string.h>
#include "raw_hid.h"
#include "wait.h"
#include "./layers.c"
#include "./custom_keycodes.c"

void vim_insert(void) {
    enable_vim_mode();
    insert_mode();
}

DEFINE_SERIAL_COMMANDS({
    { "neovide", &disable_vim_mode },
    { WT("google-chrome", "*claude - google*"), &vim_insert, &disable_vim_mode },
    { WT("google-chrome", "*chatgpt - google*"), &vim_insert, &disable_vim_mode },
    { WT("google-chrome", "deepseek* - google*"), &vim_insert, &disable_vim_mode },
});

DEFINE_SERIAL_LAYERS({
    { "*calculator", _NUMPAD },
});
