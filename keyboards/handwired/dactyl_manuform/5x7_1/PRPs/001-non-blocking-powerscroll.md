# Non-Blocking Powerscroll Implementation PRP

---

## Goal

**Feature Goal**: Transform the existing blocking powerscroll feature into a non-blocking implementation that allows other key events to be processed while powerscroll is active, while maintaining the same user experience and functionality.

**Deliverable**: Modified powerscroll.c with non-blocking timer-based implementation that eliminates wait_ms() calls and integrates with existing matrix_scan_user() pattern.

**Success Definition**: Powerscroll continues sending scroll events with 9-10ms delays without blocking other keyboard functionality - users can type and use other keys while powerscroll is running.

## User Persona (if applicable)

**Target User**: Power users who frequently use the powerscroll feature while needing to perform other keyboard actions simultaneously.

**Use Case**: User activates powerscroll to rapidly navigate through long documents, code files, or web pages while occasionally needing to press other keys (arrows, escape, shortcuts) without losing the scroll momentum.

**User Journey**: 
1. User holds POWERSCROLL_HOLD key to activate powerscroll mode
2. User rotates encoder which triggers rapid repeated scroll events with delays
3. User presses other keys (arrows, shortcuts, typing) which are processed immediately
4. Powerscroll continues running in background until user releases POWERSCROLL_HOLD

**Pain Points Addressed**: Eliminates keyboard freeze during powerscroll operation, allowing multitasking and preventing user frustration when they need to interrupt scrolling.

## Why

- **Eliminates blocking behavior**: Current implementation freezes entire keyboard for up to 2.3 seconds during powerscroll
- **Maintains responsive keyboard**: Users can press other keys without losing scroll momentum
- **Preserves existing functionality**: Same user interface and behavior, just non-blocking
- **Improves user experience**: No more keyboard lockup during rapid scrolling operations
- **Follows QMK best practices**: Uses proper timer-based non-blocking patterns

## What

Transform powerscroll from blocking synchronous operation to non-blocking state-machine driven implementation:

### Current Behavior (Blocking):
- User triggers encoder event → `tap_code16_n_times()` → loops with `wait_ms(9)` → keyboard frozen
- All other key processing blocked during scroll sequence

### New Behavior (Non-blocking):
- User triggers encoder event → schedule scroll sequence → immediate return to main loop
- Background timer system sends individual scroll events with 9ms intervals
- Other keys processed normally while scroll events continue
- Same visual/application behavior as before

### Success Criteria

- [ ] Powerscroll maintains same scroll speed and timing (9-10ms intervals)
- [ ] Other keys process immediately while powerscroll is active
- [ ] No keyboard lockup during any powerscroll operation
- [ ] Encoder stacking behavior preserved (multiple POWERSCROLL_HOLD keys)
- [ ] All existing powerscroll configurations work unchanged
- [ ] No performance degradation in matrix scan cycle

## All Needed Context

### Context Completeness Check

_If someone knew nothing about this codebase, would they have everything needed to implement this successfully?_

**Answer**: Yes - this PRP provides complete current implementation analysis, exact patterns to follow, integration points, and specific technical requirements.

### Documentation & References

```yaml
# QMK Timer Documentation
- url: https://docs.qmk.fm/#/custom_quantum_functions
  why: Official QMK documentation on timer functions and matrix scan hooks
  critical: timer_read(), timer_elapsed() usage patterns for non-blocking timing

- url: https://github.com/qmk/qmk_firmware/blob/master/quantum/deferred_exec.h
  why: Modern QMK deferred execution API - alternative implementation approach
  critical: Callback return values control repeat behavior (0=stop, >0=repeat after N ms)

# Existing Codebase Patterns
- file: keymaps/default/custom_mod_map.c
  why: Established timer pattern using timer_read() and matrix_scan_user() integration
  pattern: ModState struct with time_activated field, matrix_scan_user() processing loop
  gotcha: Uses timer_read() - mod_states[i].time_activated for elapsed time calculation

- file: keymaps/default/powerscroll.c  
  why: Current blocking implementation to transform
  pattern: tap_code16_n_times() function, encoder event processing, hold count tracking
  gotcha: wait_ms() calls block entire keyboard - must be eliminated

- file: keymaps/default/powerscroll.h
  why: Public interface and configuration defines
  pattern: POWERSCROLL_DELAY_MS (9ms), POWERSCROLL_LEVEL (10), extern function declarations
  gotcha: Interface must remain unchanged for compatibility

- file: keymaps/default/keymap.c
  why: Integration point showing powerscroll_process() hook in process_record_user()
  pattern: Clean integration into existing processing chain
  gotcha: Must preserve existing integration - only internal implementation changes
```

### Current Codebase Structure

```bash
keymaps/default/
├── powerscroll.c          # Current blocking implementation (TO MODIFY)
├── powerscroll.h          # Public interface (KEEP UNCHANGED)  
├── custom_mod_map.c       # Timer pattern to follow
├── keymap.c               # Integration point
├── custom_keycodes.c      # Other keyboard functionality
├── config.h               # POWERSCROLL_DELAY_MS = 9, POWERSCROLL_LEVEL = 10
└── rules.mk               # Build configuration
```

### Desired Codebase Structure

```bash
keymaps/default/
├── powerscroll.c          # MODIFIED: Non-blocking state machine implementation
├── powerscroll.h          # UNCHANGED: Same public interface
├── custom_mod_map.c       # MODIFIED: matrix_scan_user() enhanced with powerscroll processing
└── (all other files unchanged)
```

### Known Gotchas & Library Quirks

```c
// CRITICAL: Timer overflow handling
// timer_read() returns uint16_t - overflows every ~65 seconds
// Use timer_elapsed(start_time) instead of direct subtraction for safety

// CRITICAL: Matrix scan frequency  
// matrix_scan_user() called ~1000Hz - keep powerscroll processing lightweight
// Avoid complex calculations or multiple timer checks per scan

// CRITICAL: Encoder event timing
// IS_ENCODEREVENT() macro detects encoder events carrying mapped keycodes
// Events can arrive at high frequency during fast rotation

// GOTCHA: Existing integration
// powerscroll_process() called from process_record_user() - must preserve this interface
// Only internal implementation changes, external behavior identical

// GOTCHA: Hold count stacking
// s_powerscroll_hold_count can be >1 when multiple POWERSCROLL_HOLD keys pressed
// Exponential multiplier: repeats = POWERSCROLL_LEVEL ^ s_powerscroll_hold_count (max 255)
```

## Implementation Blueprint

### Data Models and Structure

```c
// New non-blocking state structure
typedef struct {
    bool is_active;              // Powerscroll currently running
    uint16_t last_tap_time;      // Last time we sent a scroll event (timer_read() format)
    uint8_t remaining_taps;      // How many more scroll events to send
    uint16_t keycode;            // Which keycode to send (KC_MS_WH_UP, etc.)
    uint8_t tap_interval_ms;     // Delay between taps (POWERSCROLL_DELAY_MS)
} PowerscrollState;

// Global state instance (replace current static variables)
static PowerscrollState powerscroll_state = {
    .is_active = false,
    .last_tap_time = 0,
    .remaining_taps = 0,
    .keycode = KC_NO,
    .tap_interval_ms = POWERSCROLL_DELAY_MS
};
```

### Implementation Tasks (ordered by dependencies)

```yaml
Task 1: MODIFY keymaps/default/powerscroll.c
  - IMPLEMENT: PowerscrollState struct and global state management
  - REPLACE: tap_code16_n_times() with powerscroll_schedule_taps()
  - REMOVE: All wait_ms() calls - make functions non-blocking
  - PRESERVE: Public interface functions (powerscroll_process, powerscroll_handle_encoder)
  - PATTERN: Follow custom_mod_map.c timer usage (timer_read() storage, timer_elapsed() checks)
  - NAMING: Keep existing function names, add powerscroll_matrix_scan() for timer processing

Task 2: MODIFY keymaps/default/custom_mod_map.c  
  - INTEGRATE: Add powerscroll_matrix_scan() call to existing matrix_scan_user()
  - PRESERVE: All existing mod state processing  
  - PATTERN: Add powerscroll processing after existing mod loop
  - PLACEMENT: keymaps/default/custom_mod_map.c:matrix_scan_user() function

Task 3: MODIFY keymaps/default/powerscroll.h
  - DECLARE: powerscroll_matrix_scan() function for matrix scan integration
  - PRESERVE: All existing declarations unchanged
  - PATTERN: Follow existing extern declarations in header
  - NAMING: Use existing powerscroll_ prefix convention

Task 4: VALIDATE integration points
  - VERIFY: process_record_user() integration unchanged in keymap.c
  - TEST: Encoder event processing with IS_ENCODEREVENT() macro
  - PRESERVE: All existing powerscroll trigger behavior
  - PATTERN: Maintain exact same user experience and interface
```

### Implementation Patterns & Key Details

```c
// Core non-blocking scheduling function (replaces tap_code16_n_times)
static void powerscroll_schedule_taps(uint16_t keycode, uint8_t times) {
    if (times == 0) return;
    
    // Initialize state for non-blocking execution
    powerscroll_state.is_active = true;
    powerscroll_state.remaining_taps = times;
    powerscroll_state.keycode = keycode;
    powerscroll_state.tap_interval_ms = POWERSCROLL_DELAY_MS;
    powerscroll_state.last_tap_time = timer_read();  // Store start time
    
    // Send first tap immediately for responsiveness
    tap_code16(keycode);
    powerscroll_state.remaining_taps--;
}

// Matrix scan processing function (called from matrix_scan_user)
void powerscroll_matrix_scan(void) {
    if (!powerscroll_state.is_active) return;
    
    // Check if enough time has elapsed for next tap
    if (timer_elapsed(powerscroll_state.last_tap_time) >= powerscroll_state.tap_interval_ms) {
        if (powerscroll_state.remaining_taps > 0) {
            // Send next scroll event
            tap_code16(powerscroll_state.keycode);
            powerscroll_state.remaining_taps--;
            powerscroll_state.last_tap_time = timer_read();  // Reset timer
        } else {
            // Sequence complete
            powerscroll_state.is_active = false;
        }
    }
}

// Matrix scan integration (add to existing matrix_scan_user in custom_mod_map.c)
void matrix_scan_user(void) {
    // Existing mod state processing
    for (int i = 0; i < MAX_MOD_KEYS; i++) {
        int tapping_term = get_tapping_term(mod_states[i].keycode, NULL);
        if (mod_states[i].is_active && timer_read() - mod_states[i].time_activated > tapping_term) {
            mod_states[i].is_held = true;
            register_code(mod_states[i].mod_code);
        }
    }
    
    // Add powerscroll processing
    powerscroll_matrix_scan();
}
```

### Integration Points

```yaml
MATRIX_SCAN:
  - modify: keymaps/default/custom_mod_map.c:matrix_scan_user()
  - action: "Add powerscroll_matrix_scan() call after existing mod processing loop"
  - pattern: "Follow existing timer-based processing approach"

HEADER_INTEGRATION:
  - modify: keymaps/default/powerscroll.h  
  - action: "Add powerscroll_matrix_scan() declaration"
  - pattern: "extern void powerscroll_matrix_scan(void);"

STATE_MANAGEMENT:
  - modify: keymaps/default/powerscroll.c
  - action: "Replace static hold count with PowerscrollState struct"
  - pattern: "Follow ModState struct pattern from custom_mod_map.c"
```

## Validation Loop

### Level 1: Syntax & Style (Immediate Feedback)

```bash
# Build validation after each file modification
qmk compile -kb handwired/dactyl_manuform/5x7_1 -km default

# Expected: Clean compilation with no errors or warnings
# If compilation fails: Read error output and fix syntax issues before proceeding
```

### Level 2: Component Testing (Functional Validation)

```bash
# Flash firmware to keyboard for testing
qmk flash -kb handwired/dactyl_manuform/5x7_1 -km default

# Test powerscroll functionality:
# 1. Hold POWERSCROLL_HOLD key, rotate encoder - should see smooth scrolling
# 2. While powerscroll is running, press other keys - should register immediately  
# 3. Test encoder in different layers (NUMPAD, FN, CMD) - all should work
# 4. Test multiple POWERSCROLL_HOLD keys simultaneously - should stack properly

# Expected: All powerscroll functionality works as before, but keyboard remains responsive
```

### Level 3: Timing Validation (Performance Testing)

```bash
# Enable console output for debugging
qmk console

# Add debug prints to powerscroll_matrix_scan() to verify timing:
# - Check that taps occur every ~9ms as expected
# - Verify no timing drift over long sequences  
# - Confirm matrix scan frequency isn't impacted

# Performance test:
# 1. Activate powerscroll with maximum repeats (255 taps)
# 2. Simultaneously type characters and use other keys
# 3. Monitor console output for timing accuracy

# Expected: 9ms intervals maintained, other keys process immediately
```

### Level 4: Integration Testing (System Validation)

```bash
# Comprehensive functionality testing:

# Test 1: Basic powerscroll operation
# - Hold POWERSCROLL_HOLD, rotate encoder in each direction
# - Verify smooth scrolling without keyboard lockup

# Test 2: Interrupt testing  
# - Start long powerscroll sequence (hold multiple POWERSCROLL_HOLD keys)
# - Press escape, arrows, function keys while scrolling
# - Verify all keys register immediately

# Test 3: Layer switching during powerscroll
# - Start powerscroll in QWERTY layer
# - Switch to NEOVIM, BROWSER, or TERMINAL layers
# - Verify layer switching works, powerscroll continues

# Test 4: Multiple encoder testing
# - Test both encoders simultaneously with powerscroll
# - Verify independent operation

# Expected: All tests pass with responsive keyboard and proper powerscroll timing
```

## Final Validation Checklist

### Technical Validation

- [ ] Clean compilation: `qmk compile -kb handwired/dactyl_manuform/5x7_1 -km default`
- [ ] Successful flash: `qmk flash -kb handwired/dactyl_manuform/5x7_1 -km default`  
- [ ] No regression in existing functionality
- [ ] Powerscroll timing maintained at 9ms intervals
- [ ] Matrix scan performance unimpacted

### Feature Validation

- [ ] Powerscroll works in all encoder layers (QWERTY, NUMPAD, FN, CMD, MOUSE)
- [ ] Hold count stacking preserved (multiple POWERSCROLL_HOLD keys)
- [ ] Scroll speed calculation unchanged (POWERSCROLL_LEVEL ^ hold_count, max 255)
- [ ] Other keys process immediately during powerscroll operation
- [ ] No keyboard lockup during any powerscroll sequence
- [ ] Encoder events processed correctly with IS_ENCODEREVENT() macro

### Code Quality Validation

- [ ] Follows existing timer pattern from custom_mod_map.c
- [ ] PowerscrollState struct matches ModState pattern conventions
- [ ] Public interface unchanged in powerscroll.h
- [ ] Integration cleanly added to matrix_scan_user()  
- [ ] No wait_ms() calls remaining in powerscroll code
- [ ] Timer overflow handling using timer_elapsed() function

### User Experience Validation

- [ ] Same powerscroll activation behavior (hold POWERSCROLL_HOLD + rotate encoder)
- [ ] Same visual scrolling speed and feel
- [ ] Improved responsiveness - can type while scrolling
- [ ] No noticeable performance impact during normal keyboard use
- [ ] Configuration values (POWERSCROLL_DELAY_MS, POWERSCROLL_LEVEL) work as expected

---

## Anti-Patterns to Avoid

- ❌ Don't use wait_ms() or any blocking delays in the implementation
- ❌ Don't modify the public interface in powerscroll.h
- ❌ Don't change the user experience or activation behavior  
- ❌ Don't add complex calculations to matrix_scan_user() - keep it lightweight
- ❌ Don't use direct timer arithmetic - use timer_elapsed() for overflow safety
- ❌ Don't break encoder event processing with IS_ENCODEREVENT() macro
- ❌ Don't interfere with existing mod state processing in custom_mod_map.c