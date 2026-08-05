#!/usr/bin/env python3
"""
Generate a VIA keyboard-definition JSON (for side-loading via the VIA app's
Design tab) from this keyboard's data-driven `keyboard.json` layout.

The VIA `keymap` is KLE-format: an array of rows, each key labeled with its
switch-matrix coordinate "row,col". This script converts the absolute (x,y,w)
coordinates in keyboard.json into KLE-relative rows with x-offsets.

Re-run after editing the layout in keyboard.json:
    python3 gen_via_definition.py
"""
import json
import re
import sys
from pathlib import Path

KB_DIR = Path(__file__).resolve().parent
INFO = KB_DIR / "keyboard.json"
OUT  = KB_DIR / "via_definition.json"
CUSTOM_H = KB_DIR / "keymaps" / "default" / "custom_keycodes.h"


def parse_custom_keycodes(path: Path) -> list[str]:
    """Pull the ordered custom-keycode names from the default keymap's enum.

    The enum is rebased to QK_KB_0 (0x7E00) so VIA's customKeycodes array —
    indexed from the app's only custom base, 0x7E00 — lines up entry-for-entry.
    """
    m = re.search(r"enum custom_keycodes \{(.+?)\};", path.read_text(), re.S)
    names: list[str] = []
    for entry in m.group(1).split(","):
        entry = re.sub(r"//.*", "", entry).strip()          # strip comments
        if not entry:
            continue
        im = re.match(r"([A-Z][A-Z0-9_]*)", entry)         # first identifier only
        if im:
            names.append(im.group(1))
    return names


def humanize(ident: str) -> str:
    return " ".join(w.capitalize() for w in ident.split("_"))


def abbrev(ident: str) -> str:
    return "".join(w[0].upper() for w in ident.split("_") if w)[:5]


def main() -> int:
    info = json.loads(INFO.read_text())
    layout = info["layouts"]["LAYOUT_5x7_1"]["layout"]

    # VIA's matrix must match QMK MATRIX_ROWS/MATRIX_COLS (12x7 for this split).
    max_row = max(k["matrix"][0] for k in layout)
    max_col = max(k["matrix"][1] for k in layout)

    # Group keys into KLE rows by integer y, sort each row by x.
    rows: dict[int, list[dict]] = {}
    for k in layout:
        rows.setdefault(int(k["y"]), []).append(k)

    keymap = []

    # --- Encoders (one per half) ---
    # The rotary knobs are NOT scanned by the matrix (rotation lives on
    # GP28/GP29), but their push-buttons ARE real matrix keys (MS_BTN4=[4,6]
    # left, MS_BTN5=[10,0] right). So each knob is drawn at its push-button's
    # matrix coordinate and tagged with the VIA encoder marker `eN` (label
    # "<row>,<col>" + 9x"\n" + "e<index>"). VIA then exposes three bindings per
    # knob: Rotate CCW, Rotate CW (the encoder protocol, by index), and **Press
    # Encoder** (the ordinary keymap key at this matrix coord = the physical
    # push). This is why the push is wired into the matrix — VIA's "Press" field
    # *is* that matrix key.
    #
    # Placement: the free y=4 row — left knob under PgDn [3,6], right knob under
    # the Down-arrow [9,0]. The push-button coords are absorbed into the knobs
    # (not rendered as separate plain keys) so the layout shows a knob, not a
    # knob + a redundant keycap beside it.
    ENCODER_Y = 4
    encoders = [
        {"matrix": (4, 6), "index": 0, "x": 6.0},    # left knob = MS_BTN4 push
        {"matrix": (10, 0), "index": 1, "x": 10.0},  # right knob = MS_BTN5 push
    ]
    encoder_matrix = {enc["matrix"]: enc["index"] for enc in encoders}

    # Render real keys (EXCEPT the encoder-push coords, which the knobs absorb)
    # + the encoder knobs, grouped by integer y, sorted by x within each row.
    # KLE resets the x cursor to 0 at the start of each row array.
    keymap = []
    for y in sorted(set(rows) | {ENCODER_Y}):
        items = []
        for k in rows.get(y, []):
            m = (k["matrix"][0], k["matrix"][1])
            if m in encoder_matrix:
                continue  # rendered as an encoder knob below, not a plain key
            items.append({"x": k["x"], "w": k.get("w", 1), "matrix": k["matrix"], "enc": None})
        for enc in encoders:
            if y == ENCODER_Y:
                items.append({"x": enc["x"], "w": 1, "matrix": enc["matrix"], "enc": enc["index"]})
        items.sort(key=lambda it: (it["x"], it["matrix"][0], it["matrix"][1]))

        row_items = []
        cursor_x = 0  # KLE cursor resets to x=0 at the start of each row array
        for it in items:
            dx = it["x"] - cursor_x
            props = {}
            if it["enc"] is not None:
                props["fa"] = [1, 0, 0, 0, 0, 0, 0, 0, 0, 1]  # show coord + eN, hide gaps
            if dx != 0:
                props["x"] = dx
            if it["w"] != 1:
                props["w"] = it["w"]
            if props:
                row_items.append(props)
            r, c = it["matrix"]
            if it["enc"] is not None:
                row_items.append(f"{r},{c}" + "\n" * 9 + f"e{it['enc']}")
            else:
                row_items.append(f"{r},{c}")
            cursor_x = it["x"] + it["w"]
        keymap.append(row_items)

    custom_names = parse_custom_keycodes(CUSTOM_H)
    custom_keycodes = [
        # VIA V3 schema allows only {name, shortName, title} per entry
        # (name + title required). Older drafts used `short`; the current app
        # rejects it with "must NOT have additional properties".
        {"name": humanize(n), "shortName": abbrev(n), "title": humanize(n)}
        for n in custom_names
    ]

    # USB VID/PID are read from keyboard.json's `usb` block so the firmware and
    # the VIA definition can never drift apart: change it once in keyboard.json
    # (usb.vid / usb.pid), re-run this script, then reflash.
    usb = info.get("usb", {})
    vendor_id = usb.get("vid", "0xFEED")
    product_id = usb.get("pid", "0x0000")

    # VIA refuses to load any definition whose vendorId is 0xFEED (QMK's
    # placeholder — shared by countless default boards, so it can't identify a
    # device). The guard lives in @the-via/reader's getVendorProductId() and
    # fires for BOTH V2 and V3, so toggling "Use V2 definitions" won't bypass
    # it. Pick a real/unique VID:PID in keyboard.json, reflash, reconnect.
    if vendor_id.upper() == "0XFEED":
        print(
            "\033[33m"
            "WARNING: usb.vid is 0xFEED — VIA will reject this definition with\n"
            "\"'0xFEED' is not a valid vendorId.\" Set a unique VID:PID in\n"
            "keyboard.json (usb.vid/usb.pid), re-run this script, then reflash.\n"
            "See: https://github.com/qmk/qmk_firmware/issues/13174"
            "\033[0m",
            file=sys.stderr,
        )

    # NOTE: no top-level `lighting` key. VIA V3 deprecated it (it's now driven
    # by the optional `keycodes` array, e.g. ["qmk_rgblight_keycodes"]). This
    # board has no RGB/backlight features, so we omit it entirely; including
    # "lighting": "none" makes the V3 validator fail with
    # "Object: must NOT have additional properties".
    definition = {
        "name": info.get("keyboard_name", "Dactyl Manuform 5x7-1"),
        "vendorId": vendor_id,
        "productId": product_id,
        "matrix": {"rows": max_row + 1, "cols": max_col + 1},
        "customKeycodes": custom_keycodes,
        "layouts": {
            "keymap": keymap,
        },
    }

    OUT.write_text(json.dumps(definition, indent=2) + "\n")
    print(f"Wrote {OUT} ({len(layout)} keys, {len(keymap)} rows, "
          f"matrix {max_row+1}x{max_col+1}, {len(custom_keycodes)} custom keycodes @0x7E00)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
