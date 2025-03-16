void vim_insert(void) {
    enable_vim_mode();
    insert_mode();
}

// vim_lazy_insert exists because going into insert mode kills all your modifier keys.
//   If if you were fipping through workspaces quickly and activate a window,
//   with vim mide you'd be forced to unpress and re-press all your mod keys
bool wants_vim_insert = false;
uint8_t held_count = 0;

void vim_lazy_insert(void) {
    wants_vim_insert = true;
}

void vim_do_lazy_insert(void) {
    vim_insert();
    wants_vim_insert = false;
}

void check_vim_lazy_insert(bool pressed) {
    if (pressed) {
        held_count++;
    } else {
        held_count--;
    }

    if (wants_vim_insert && held_count == 0) {
        vim_do_lazy_insert();
    }
}


