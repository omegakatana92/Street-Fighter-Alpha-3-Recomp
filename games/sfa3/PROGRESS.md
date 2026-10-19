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

This set's coinage is **two coins to a credit** (the title counts the coins towards the next credit up,
"INSERT 1 MORE COIN"), so one coin on its own buys nothing; that is the game's own setting and not the
port's doing.

## How the game's own memory is found

The tools used, in the order they are reached for (all in `docs/FEATURES.md`):

- `--movie FILE` replays a hand-written input movie — a line `frame player1 player2`, the buttons as
  bits (`machine.h`), held until the next line — so the game is driven to a known place and left there:
  coins in, start, through the character and ISM screens to a match, then moves at known frames. The
  movies this work used are the throwaway ones in `work/find/` (`m1.txt`, `gm5.txt`, …), not kept.
- `--poke ADDR=BYTE` holds a byte at a guess after every frame, and `--menu FRAME:KEYS` drives the
  in-game menu with no hand (a coin, a save, the game's own commands).
- `--dump-ram FRAME:FILE` writes the 64 KiB of work RAM after a frame; `--trace LOW-HIGH:FILE` appends a
  stretch of work RAM (or all of it) after every frame; `--shot-every N:PFX` writes pictures of the board;
  `--keys TICK:cNAME` runs one of the game's own commands (`training_mode`) without a hand.
- `cps2recomp/tools/ramfind.py` lists the addresses that held a known value in each of several dumps;
  `cps2recomp/tools/tracechanges.py` prints a trace where a value changes.

A dump's byte `o` is the 68000's address `0xFF0000 + o`. The player records sit in `0xFF8400`…`0xFF8FFF`,
so `--trace 8000-8FFF:FILE` (4 KiB a frame) holds both fighters and is what nearly all of this was read
from; kept beside it, the pictures the same run wrote say what the game was showing at a frame.

## The memory so far

| Address | Size | What | How it was found |
| --- | --- | --- | --- |
| `FF8076` | byte | **Credits**: what the title reads out as "CREDIT:nn(c/s)" and a start takes one; 0 in the attract show, and what the title waits for before "PRESS 1P OR 2P START" | Played movies putting in one, two and four coins (`--movie`); the byte that read 0, 1 and 2 with them is this one (`FF0631`, `FF0CCF` and `FF0D4F` moved with it but were the coins' own audit counters, and `FF80C3` only ever climbed). Held at 2 with `--poke`, the title showed "PRESS 1P OR 2P START" with no coin, which is what `free_play` does. |
| `+0x50` (`FF8450`/`FF8850`) | word | Player 1 (player 2) **health**: `0x90` at a round's start, steps down as hits land, `0xFFFF` once the round is lost, 0 outside a round (and in the attract show) | `--trace`d a match the port played back and watched it fall `0x0090 → 0x0090 → … → 0x0049 → 0xFFFF` as player 1 was beaten while player 2's stood; the training readout below shows the same number on the picture. FinalBurn Neo's collection names the same address and `0x90`. |
| `+0x52` | word | The same health again: **what the bars are drawn from**, draining a step at a time | Trails the health word on every hit (both were `0x0090`, then `0x0088`, `0x007C` …). FinalBurn Neo's collection sets both together. |
| `+0x10` (`FF8410`/`FF8810`) | word | **Position across the stage**: larger is further right; the fighters stop at the walls | `--trace`d both records over the attract show's matches and over a match played back with known inputs: the word that ran smoothly with the fighters (and, with the stick held right from frame 4700, walked player 1 to `0x03E8` and pinned it there). |
| `+0x14` | word | **Height above the floor**: about `0x28` standing, lifted by a jump, dipped a little by a crouch | The same trace: it rose to `0x0079` and fell back over a jump. |
| `+0x0A` | word | **Facing**: 1 while the opponent is to its right, 0 while to its left | The same trace, checked against both records' X over a long match (it changed exactly as the two crossed). |
| `+0x06` | word | **The game's state** (what a player is doing) | `--trace`d a match with known inputs. Found, but only partly read: `0x0002` stands, `0x04xx` walks, `0x06xx` is in the air. The low byte and the attack, hit and guard states are not yet told apart, which is what the remaining training aids wait on. |
| `+0x11F` (`FF851F`/`FF891F`) | byte | The **LEVEL (POWER) bar**: `0x90` with the bar at MAX, 0 while empty | Held it at 0 with `--poke` and watched both bars empty on the picture. FinalBurn Neo's collection names the same address and value. |
| `FF8109` | byte | **The round's clock**: counted down and drawn just as it reads — `0x5A` is the "90" on the picture, so it is plain and not BCD; `0x63` (99) is the longest a round runs; 0 outside a round | `--trace`d a round and read it beside the clock on the picture (frame 4750 read `0x5F` with "95" shown, frame 5000 `0x5A` with "90"). FinalBurn Neo's collection names the same address. |

Both cheat collections this project reaches for were wanted for a second opinion; FinalBurn Neo's
(`cheats/sfa3.ini`, the parent set) was read for all of the above and is said where it agreed. MAME's
published collection could not be fetched from this machine (the site refuses the request), so the
cross-check here is FinalBurn Neo's alone — every address was in any case first found and then confirmed
by experiment on the board, which is what the tools above are for.

## Features built on it (`src/`)

| Feature / command | Category | What it does |
| --- | --- | --- |
| `free_play` | Game | Holds the credits at 2, so a start works with no coin (and a second player can always join). |
| `no_damage` | Cheats | Puts player 1's health back to the round's full value every frame (`+0x50` and its drawn copy `+0x52`). |
| `no_damage_2` | Cheats | The same for player 2 (the opponent's, in a one-player game). |
| `infinite_time` | Cheats | Holds the round's clock (`FF8109`) where it stood when the round began, so it never runs out. |
| `one_hit` | Cheats | Leaves player 2 a single point of health, so any hit ends the round. |
| `full_gauge` | Cheats | Holds both players' LEVEL (POWER) bars full (`+0x11F`). |
| `input_display` | Training | What each player holds, and for how long, in players' notation, turned by the game's own facing word (`+0x0A`). |
| **Training mode** (command `training_mode`) | Training | Starts a session: both fighters' health and POWER bars are held full (from the round's start, which the aiding code learns by watching the health) and the round's clock stands, and a readout beside the picture shows each fighter's health and the last hit taken — the damage being the health the game took off before it is put back. |
| **End training** (command `end_training`) | Training | Ends the session; the round then runs as it is again. |

The cheats are the switches the launcher's Features page and the in-game menu set; `Training mode` is a
command (a row of the Training page). With every feature off, and no session, the game runs exactly as
the board does: nothing in these sources writes to the game's memory unless a feature is on.

## What is not built yet, and what it waits on

The fight/ parts the toolkit supplies want the game to say what each player is *doing* a frame at a time
(`fight::StepView`: free, attacking, in the air, frozen by a hit, and so on). That is the state word at
`+0x06`, found but not yet read out: its low byte and its attack, hit and guard values have to be told
apart before

- the **frame data** meter (`fight::Frames`),
- the **combo readout** (`fight::Combos`), which also wants to know when a player has got free,
- the **dummy** (`fight::Dummy`), which has to know a hit is coming and when the dummy is knocked down,
- the **hit boxes** (`fight::StageBox`), which want the game's attack and hurt boxes found,

can be put together. The input display and the training session above need none of that.

The state word was watched (`--trace`) over the attract show's matches and over a match played back with
known inputs; the values `0x0002`, `0x04xx` and `0x06xx` above were read off against the picture, and the
rest of the word — the low byte's meaning, and which values mean an attack, a hit and a guard — is the
next thing to pin down.

## Next

- Tell the state word's values apart with a match driven by `--movie` at known frames (stand, crouch,
  walk, jump, each attack, taking a hit, guarding), reading `+0x06` beside `--shot-every` pictures; that
  unlocks the frame data, the combo readout, the dummy and the hit boxes in turn.
- Translate the rest of the game's code: `make explore` (and the ladder runs) drive the made-up play
  further, so the count of instructions with no translation keeps falling.
