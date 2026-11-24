void vim_insert(void) {
    enable_vim_mode();
    insert_mode();
}

// vim_lazy_insert exists because going into insert mode kills all your modifier keys.
//   If if you were fipping through workspaces quickly and activate a window,
//   with vim mide you'd be forced to unpress and re-press all your mod keys
bool wants_vim_insert = false;

void vim_lazy_insert(void) {
    #ifdef CONSOLE_ENABLE // Logging
        uprintf("sending vim lazy insert trigger\n");
    #endif
    wants_vim_insert = true;
}

void vim_do_lazy_insert(void) {
    #ifdef CONSOLE_ENABLE // Logging
        uprintf("entering vim mode\n");
    #endif
    vim_insert();
    wants_vim_insert = false;
}

// Hands-off detection reads the live matrix instead of tallying press/release
// events. The old held_count counter desynced permanently the first time ANY
// machinery produced an unpaired event (tap-dance deferral, mod-tap interrupt,
// leader replay, split-transport hiccup) — after which vim mode could never
// engage again. Matrix state is self-healing: it can't accumulate error.
static bool matrix_all_up(void) {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        if (matrix_get_row(row) != 0) return false;
    }
    return true;
}

void check_vim_lazy_insert(void) {
    if (wants_vim_insert && matrix_all_up()) {
        vim_do_lazy_insert();
    }
}

void disable_vim(void) {
    #ifdef CONSOLE_ENABLE // Logging
        uprintf("disabling vim mode\n");
    #endif
    wants_vim_insert = false;
    disable_vim_mode();
}
