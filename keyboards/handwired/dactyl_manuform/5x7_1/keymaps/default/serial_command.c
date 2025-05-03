#include "./qmk-vim/src/vim.h"
#include "./qmk-vim/src/modes.h"
#include <string.h>
#include "raw_hid.h"
#include "wait.h"
#include "./layers.c"
#include "./custom_keycodes.c"
#include "./vim_lazy_insert.c"

extern void vim_lazy_insert(void);

DEFINE_SERIAL_COMMANDS({
    { "neovide", &disable_vim_mode },
    { "alacritty", &disable_vim_mode },
    { "*iterm*", &disable_vim_mode },
    { "*claude*", &vim_lazy_insert, &disable_vim_mode },
    { WT("*chrome*", "*claude*"), &vim_lazy_insert, &disable_vim_mode },
    { WT("*chrome*", "*chatgpt*"), &vim_lazy_insert, &disable_vim_mode },
    { WT("*chrome*", "*deepseek*"), &vim_lazy_insert, &disable_vim_mode },
    { WT("*chrome*", "*gemini*"), &vim_lazy_insert, &disable_vim_mode },
    { WT("*brave*", "*claude*"), &vim_lazy_insert, &disable_vim_mode },
    { WT("*brave*", "*chatgpt*"), &vim_lazy_insert, &disable_vim_mode },
    { WT("*brave*", "*deepseek*"), &vim_lazy_insert, &disable_vim_mode },
    { WT("*brave*", "*gemini*"), &vim_lazy_insert, &disable_vim_mode }
});

DEFINE_SERIAL_LAYERS({
    { "*calculator", _NUMPAD },
    { WT("*chrome*", "*jitsi*"), _JITSI },
    { WT("alacritty", "terminal"), _TERMINAL },
    { WT("alacritty", "alacritty"), _TERMINAL },
    { "*iterm*", _TERMINAL },
    { WT("*alacritty*", "*matterhorn*"), _MATTERHORN },
    { "*clickup*", _CLICKUP },
    { "*neovide*", _NEOVIM },
    { "*chrome*", _CHROME },
    { "*brave*", _CHROME },
    { "*inkscape*", _INKSCAPE }
});
