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
    { "neovide", &disable_vim },
    { WT("^alacritty$", "^terminal$"), &disable_vim },
    { WT("^alacritty$", "alacritty"), &disable_vim },
    { "*iterm*", &disable_vim },
    { WT("^Claude$", "^Claude$"), &vim_lazy_insert, &disable_vim }, // claude desktop app
    { WT("*chrome*", "*claude*"), &vim_lazy_insert, &disable_vim }, // claude.ai
    { WT("*chrome*", "*chatgpt*"), &vim_lazy_insert, &disable_vim },
    { WT("*chrome*", "*deepseek*"), &vim_lazy_insert, &disable_vim },
    { WT("*chrome*", "*gemini*"), &vim_lazy_insert, &disable_vim },
    { WT("^brave-browser$", "*claude*"), &vim_lazy_insert, &disable_vim },
    { WT("^brave-browser$", "*chatgpt*"), &vim_lazy_insert, &disable_vim },
    { WT("^brave-browser$", "*deepseek*"), &vim_lazy_insert, &disable_vim },
    { WT("^brave-browser$", "gemini*"), &vim_lazy_insert, &disable_vim },
    { WT("^brave-browser$", "*ai*studio*"), &vim_lazy_insert, &disable_vim },
    { WT("^brave-browser$", "*ai*studio*"), &vim_lazy_insert, &disable_vim },
    { WT("^brave-browser$", "^zoho mail"), &vim_lazy_insert, &disable_vim },
    { "Mulletware Wiki", &vim_lazy_insert, &disable_vim },
    { WT("*", "*orderlands*"), &disable_vim },
    { WT("steam_app*", "*"), &disable_vim },
    { WT("cs2", "Counter-Strike 2"), &disable_vim },
});

DEFINE_SERIAL_LAYERS({
    { "*calculator", _NUMPAD },
    { WT("*chrome*", "*jitsi*"), _JITSI },
    { WT("^alacritty$", "^terminal$"), _TERMINAL },
    { WT("alacritty", "alacritty"), _TERMINAL },
    { "*iterm*", _TERMINAL },
    { WT("*alacritty*", "*matterhorn*"), _MATTERHORN },
    { "*clickup*", _CLICKUP },
    { "*neovide*", _NEOVIM },
    { "chrome*", _BROWSER },
    { WT("brave-browser", "*"), _BROWSER },
    { WT("firefox", "*"), _BROWSER },
    { WT("org.gnome.Nautilus", "*"), _BROWSER },
    { "*inkscape*", _INKSCAPE },
    { "borderlands*", _GAMING },
    { WT("steam_app*", "*orderlands*"), _GAMING },
    { "steam_app*", _GAMING },
    { WT("cs2", "Counter-Strike 2"), _GAMING },
});

