- To flash this project, navigate up the directory tree until you hit the `qmk` directory. There, there will be a `./flash` script you can run.
- NEVER run `./flash`, trigger the bootloader, or reboot/reset either half without an explicit go-ahead **in the current conversation** — this is daily-driver hardware and is usually in active use while an agent is working. "Go for it" / "land it" / "implement it" on a code change is NOT consent to flash. Default to `qmk compile` only; flash exactly when the user says to flash, and even then prefer handing them the command over running it.
- NEVER run mass-erase flashing commands (openocd `flash erase_address`, bossac mass erase, `--mass-erase`, `eeconfig` wipes) against any device while this keyboard is plugged in — handedness is stored in EEPROM (`EE_HANDS`) and one mass-erase kills half the keyboard, even from a session targeting a totally different board.
- If one side of the keyboard goes dead: that's lost `EE_HANDS`, not (probably) broken hardware. Restore with the field-kit SIDE_LOCK tool (`qmk-field-kit/test_sidelock.py`) over raw HID, or reflash per-side from the qmk dir. Do not re-solder anything until you've checked this first.

## Keymap development philosophy — end-user ease-of-use is the primary priority

This keymap is DX-first. Every new implementation, abstraction, module, or
snippet — no exceptions — is judged first by how it behaves the *next* time
it's touched, not by how cleverly it works today:

- **Declare once.** A behavior's data (keys, layer, target, string) appears
  exactly once, in exactly one obvious place. If adding an entry would
  require restating context — layer names per entry, wrapper/trampoline
  functions, prototype shims — that redundancy is a bug in the interface and
  must be coded away, not documented.
- **Boilerplate belongs in macros.** Hand-written glue (callback trampolines,
  generated symbol names, sentinel plumbing, per-entry bookkeeping) is an
  interface defect. Abstract until the user-facing surface is just the data.
- **No surprises.** The interface works the way it looks like it works:
  wrapper families mirror the established shapes (e.g. `SL_*` in
  super_leader.def), mismatched signatures or duplicate entries fail at
  compile time, and silent failure modes are converted into build errors.
- **Zero-docs edits.** Adding a key, sequence, layer entry, or callback must
  require reading no docs, no git history, no other source file — the file
  you edit should be the only file you touch, and the entry you write should
  be the only thing you write.
- **Favor the user over the machinery.** When convenience and internal
  elegance conflict, the user-facing surface wins; machinery may be as
  intricate as it needs to be to keep the surface plain.
