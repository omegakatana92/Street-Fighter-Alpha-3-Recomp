# Street Fighter Alpha 3 Recomp — changelog

## 0.1.0-beta.2

The game's own features, built on addresses found by experiment on the running board and recorded in
[games/sfa3/PROGRESS.md](games/sfa3/PROGRESS.md):

- **Free play**, so Start works without a coin.
- The **cheats**: no damage (either fighter), infinite time, one-hit knockouts and full POWER bars.
- **Training**: an **input display** in players' notation, and a **Training mode** session that holds
  both fighters' health and POWER full, stands the round's clock still, and reads out each fighter's
  health and the last hit taken.

The rest of the training mode (frame data, the combo readout, the dummy and the hit boxes) is still to
come: it waits on the game's per-player state being read out. See [ROADMAP.md](ROADMAP.md).

## 0.1.0-beta.1

First build. The 68000 and Z80 programs of *Street Fighter Alpha 3* (Europe 980904) are translated to
C++ ahead of time and run on cps2recomp's own machine. The launcher, the in-game menu, save states,
run-ahead, the display options and every control are in, and every key and button is remappable.

The game's own features (the cheats and the training aids the sibling cps2recomp ports have) are still
to come; they wait on addresses found on the running board, recorded in
[games/sfa3/PROGRESS.md](games/sfa3/PROGRESS.md) as they are found. See [ROADMAP.md](ROADMAP.md).

The release archives carry no game data: the player brings their own `sfa3` ROM set, which the port only
reads.
