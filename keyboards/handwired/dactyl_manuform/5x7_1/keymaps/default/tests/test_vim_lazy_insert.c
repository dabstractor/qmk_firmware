// Unit tests for the vim_lazy_insert state machine (keymaps/default/vim_lazy_insert.c).
//
// Regression backdrop (2026-11-24): the old implementation tallied press/release
// events into held_count and fired when the tally hit zero. Any unpaired event
// from QMK machinery (tap-dance deferral, mod-tap interrupt, leader replay,
// split-transport hiccup) left a phantom hold, and vim mode could never engage
// again — observed live: arm fired, all keys released, no vim entry. The fix
// derives "hands off" from the physical matrix, which cannot accumulate error.
// These tests encode that contract.
//
// Build & run: ./run_tests.sh

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// ---------------------------------------------------------------------------
// QMK stubs — just enough surface for vim_lazy_insert.c
// ---------------------------------------------------------------------------
#define MATRIX_ROWS 12
#define MATRIX_COLS 7

static uint8_t test_matrix[MATRIX_ROWS];

uint8_t matrix_get_row(uint8_t row) { return row < MATRIX_ROWS ? test_matrix[row] : 0; }

// qmk-vim stubs
static int  vim_enable_calls = 0;
static int  insert_mode_calls = 0;
static bool vim_mode_on = false;
void enable_vim_mode(void) { vim_mode_on = true; vim_enable_calls++; }
void insert_mode(void) { insert_mode_calls++; }
void disable_vim_mode(void) { vim_mode_on = false; }

// Include the unit under test (it has no includes of its own; CONSOLE off)
#include "../vim_lazy_insert.c"

// ---------------------------------------------------------------------------
// Harness
// ---------------------------------------------------------------------------
static int failures = 0;
#define CHECK(cond)                                                     \
    do {                                                                \
        if (!(cond)) {                                                  \
            printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);    \
            failures++;                                                 \
        }                                                               \
    } while (0)

static void reset(void) {
    memset(test_matrix, 0, sizeof(test_matrix));
    wants_vim_insert = false;
    vim_enable_calls = insert_mode_calls = 0;
    vim_mode_on = false;
}

static void key_down(uint8_t row) { test_matrix[row] = 1; }
static void key_up(uint8_t row) { test_matrix[row] = 0; }
static bool entered_vim(void) { return insert_mode_calls > 0; }

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------
static void test_no_arm_no_fire(void) {
    reset();
    for (int i = 0; i < 5; i++) check_vim_lazy_insert();
    CHECK(!entered_vim());
}

static void test_arm_fires_when_hands_off(void) {
    reset();
    vim_lazy_insert();      // host armed the trigger (window match)
    CHECK(wants_vim_insert);
    check_vim_lazy_insert(); // e.g. the matrix_scan_user hook
    CHECK(entered_vim());
    CHECK(vim_mode_on);
}

static void test_fires_exactly_once(void) {
    reset();
    vim_lazy_insert();
    check_vim_lazy_insert();
    int calls_after_first = insert_mode_calls;
    for (int i = 0; i < 5; i++) check_vim_lazy_insert();
    CHECK(insert_mode_calls == calls_after_first); // wants cleared, no re-entry
}

static void test_arm_waits_for_all_keys_up(void) {
    reset();
    key_down(3);
    key_down(7);
    vim_lazy_insert();      // armed mid-chord (original design intent: don't
    check_vim_lazy_insert(); // kill held mods — wait for hands off)
    CHECK(!entered_vim());
    key_up(3);
    check_vim_lazy_insert();
    CHECK(!entered_vim());  // one key still down
    key_up(7);
    check_vim_lazy_insert();
    CHECK(entered_vim());
}

// THE REGRESSION: all keys are physically released, but the event stream is
// untrusted — some QMK machinery (tap-dance deferral, mod-tap interrupt,
// leader replay, transport hiccup) may have eaten a release event. The old
// held_count tally sat at 1 forever and vim never engaged; the matrix-derived
// check fires on physical truth regardless of event accounting.
static void test_unpaired_events_do_not_wedge(void) {
    reset();
    key_down(2); key_down(5); key_down(9); // three physical presses
    vim_lazy_insert();
    check_vim_lazy_insert();
    CHECK(!entered_vim());                  // keys still physically down
    key_up(2); key_up(5); key_up(9);        // hands off — events may lie; matrix doesn't
    check_vim_lazy_insert();
    CHECK(entered_vim());                   // must fire
}

static void test_disable_clears_arm(void) {
    reset();
    vim_lazy_insert();
    disable_vim();          // focus moved to a disable-vim window
    check_vim_lazy_insert();
    CHECK(!entered_vim());
    CHECK(!vim_mode_on);
    CHECK(!wants_vim_insert);
}

static void test_rearm_after_disable(void) {
    reset();
    vim_lazy_insert();
    disable_vim();
    vim_lazy_insert();      // back to an arming window
    check_vim_lazy_insert();
    CHECK(entered_vim());
}

int main(void) {
    test_no_arm_no_fire();
    test_arm_fires_when_hands_off();
    test_fires_exactly_once();
    test_arm_waits_for_all_keys_up();
    test_unpaired_events_do_not_wedge();
    test_disable_clears_arm();
    test_rearm_after_disable();

    if (failures) {
        printf("%d FAILURE(S)\n", failures);
        return 1;
    }
    printf("all vim_lazy_insert tests passed\n");
    return 0;
}
