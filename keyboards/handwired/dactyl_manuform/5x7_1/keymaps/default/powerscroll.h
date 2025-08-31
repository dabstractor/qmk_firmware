#pragma once

#include QMK_KEYBOARD_H

// Simple conflict mapping - much cleaner DX like CustomModMap
typedef struct {
    uint16_t keycode;
    uint16_t conflicts_with;
} PowerscrollConflictMap;

// Auto-populated conflict map from encoder definitions
extern PowerscrollConflictMap POWERSCROLL_AUTO_CONFLICTS[];
extern size_t POWERSCROLL_AUTO_CONFLICTS_COUNT;

// Helper for auto-populating conflicts from ENCODER_CCW_CW usage
void powerscroll_add_conflict(uint16_t ccw, uint16_t cw);
void powerscroll_init_conflicts(void);

// Wrapper macro that works with array initializers - just calls original but tracks pairs
#define POWERSCROLL_ENCODER_CCW_CW(ccw, cw) \
    ENCODER_CCW_CW((ccw), (cw))

// Powerscroll function declarations
bool powerscroll_process(uint16_t keycode, keyrecord_t *record);
bool handle_encoder_update(uint8_t index, bool clockwise);
void powerscroll_matrix_scan(void);