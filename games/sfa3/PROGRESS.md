# Street Fighter Alpha 3: progress

What is known of *Street Fighter Alpha 3* (Europe 980904)'s memory, and what has been built on it.
Nothing here is a dump of the game's data, and no game data is written down: only where the game keeps
a thing, found by experiment on the running board with the toolkit's own tools (`docs/FEATURES.md`).

## The ROM set

The set is MAME's **`sfa3`** (Europe 980904), the parent of `sfa3u`, `sfa3ur1`, `sfa3us` (a USA sample
build), `sfa3h`, `sfa3hr1` and `sfa3b` (and the `sfa3ud` bootleg). It was told apart from the clones,
and its files listed, from the set's own contents against MAME's ROM definition for `sfa3` (Capcom,
1998, board `cps2_2p6b`, 2 players, 6 buttons):

- the program is eight 512 KiB ROMs — `sz3e.03c`, `sz3e.04c`, `sz3.05c` … `sz3.10b` — 4 MiB in all, so
  `code_end = 400000`;
- the graphics are eight 4 MiB ROMs in two rows of four — `sz3.13m`, `sz3.15m`, `sz3.17m`, `sz3.19m`
  then `sz3.14m`, `sz3.16m`, `sz3.18m`, `sz3.20m` — 32 MiB, side by side, and the graphics region is
  populated from 0 (no leading hole, unlike Street Fighter Alpha);
- the sound program is `sz3.01`, `sz3.02`; the QSound samples are `sz3.11m`, `sz3.12m` (8 MiB);
- the key is `sfa3.key`.

Every file is checked by CRC-32 (`games/sfa3/game.cfg`), so only this revision is accepted.

## How the game's own memory is found

The tools used, in the order they are reached for (all in `docs/FEATURES.md`):

- `--menu FRAME:KEYS` drives the in-game menu with no window: a coin goes in without a hand.
- `--movie FILE` replays a hand-written input movie, and `--poke ADDR=BYTE` holds a byte at a guess; the
  two together are how the game is driven to a known place and a thought tried.
- `--dump-ram FRAME:FILE` writes the 64 KiB of work RAM after a frame; `--trace LOW-HIGH:FILE` appends a
  stretch of work RAM (or all of it) after every frame; `--shot-every N:PFX` writes pictures of the board.
- `cps2recomp/tools/ramfind.py` lists the addresses that held a known value in each of several dumps;
  `cps2recomp/tools/tracechanges.py` prints a trace where a value changes.

## The memory so far

_(None yet: this port has just been translated, with `make explore` driving the game's code out of the
interpreter and into translated C++. Addresses found from here on are written in this table, and the
`src/` files name only those.)_

| Address | Size | What | How it was found |
| --- | --- | --- | --- |
| _(none yet)_ | | | |

## Features built on it (`src/`)

_(None yet. Any game-side fix — a hook for a piece of the program the port has to do differently from
the board — is added in `games/sfa3/src/` and written up here.)_

## Next

- Translate the rest of the game's code: `make explore` (and the ladder runs) drive the made-up play
  further, so the count of instructions with no translation keeps falling.
- Find the player records (health, the LEVEL bar, the round's clock) as the sibling ports do, and build
  the features the toolkit's `fight/` parts want on top of them: free play and the cheats first, then
  the training aids (input display, frame data, the combo readout, the dummy and the hit boxes).
