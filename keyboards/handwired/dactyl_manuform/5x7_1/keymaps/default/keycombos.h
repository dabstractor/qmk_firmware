#include QMK_KEYBOARD_H

#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define KEYS(...) { __VA_ARGS__ }, NUMARGS(__VA_ARGS__)

#define MAX_COMBO_KEYS 5

typedef struct {
    char keys[MAX_COMBO_KEYS + 1];
    int count;
    const char* output;
} KeyCombo;

bool leader(char *keys, int length) {
  switch(length) {
    case 1: return leader_sequence_one_key(keys[0]);
    case 2: return leader_sequence_two_keys(keys[0], keys[1]);
    case 3: return leader_sequence_three_keys(keys[0], keys[1], keys[2]);
    case 4: return leader_sequence_four_keys(keys[0], keys[1], keys[2], keys[3]);
    case 5: return leader_sequence_five_keys(keys[0], keys[1], keys[2], keys[3], keys[4]);
    default:
      return false;
  }
}
