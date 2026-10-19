// SPDX-License-Identifier: GPL-3.0-only
//
// Street Fighter Alpha 3 (Europe 980904): names for what is known of the game's memory. Every address here
// was found by experiment on the running board with the toolkit's own tools (--movie, --dump-ram, --poke,
// --trace) and checked where it could be against an independent cheat collection for the same ROM set
// (FinalBurn Neo's); PROGRESS.md says how each one was found. No game data is written down here, only where
// the game keeps it.
#pragma once

#include "game.h"

#include <cstdint>

namespace sfa3 {

// The credits the game is holding, a byte: the title reads it out as "CREDIT:nn(c/s)" and a start takes one.
// This set's coinage is two coins to a credit — the title counts the coins towards the next one up, "INSERT 1
// MORE COIN" — so one coin on its own does not move it. It is 0 in the attract show, and the title waits for
// a credit before it offers "PRESS 1P OR 2P START". Found by playing movies that put in one, two and four
// coins (--movie) and keeping the byte that read 0, 1 and 2 with them; held at 2 with --poke, the title
// offers the start (PROGRESS.md).
constexpr uint32_t kCredits = 0xFF8076;
constexpr uint8_t kCreditsFree = 2; // (two: a second player can always join)

// A player's record: the four are 0x400 apart, player 1's first (this game keeps four, for its 2-on-1 and
// 2-on-2 modes).
//   +0x50  the health, a word (0x90 at the round's start: what a hit takes bits off)
//   +0x52  the same health again, drawn down a step at a time: what the bars are drawn from
// Both are 0 outside a round, and in the attract show.
constexpr uint32_t kPlayer0 = 0xFF8400, kPlayerStride = 0x400;
constexpr uint32_t kHealth = 0x50, kHealthShown = 0x52;
inline constexpr uint32_t Player(int n) {
    return kPlayer0 + static_cast<uint32_t>(n) * kPlayerStride;
}

// A player's place in the stage, a word each: +0x10 is across it (larger is further right) and +0x14 is the
// height above the floor, which is about 0x28 (a jump lifts it, a crouch lowers it a little). Found by
// tracing both records over the attract show's matches and over a match the port played back (--movie), and
// keeping the words that ran smoothly with the fighters (PROGRESS.md).
constexpr uint32_t kPositionX = 0x10, kPositionY = 0x14;
constexpr uint16_t kFloorY = 0x28;

// Which way a player faces, a word: 1 while the opponent is to its right, 0 while to its left. Found the same
// way, checking the flag against both records' X over a long match (PROGRESS.md).
constexpr uint32_t kFacing = 0x0A;

// What a player is doing, a word: the game's own state. Found by tracing a match with known inputs (--movie)
// but its values are only partly told apart: 0x0002 stands, 0x04xx walks and 0x06xx is in the air; the low
// byte, and the attack, hit and guard states, are not yet read (PROGRESS.md).
constexpr uint32_t kState = 0x06;

// The LEVEL (POWER) bar at the bottom of the picture, a byte: it fills as the round is fought and reads 0x90
// when the bar shows MAX, 0 while it is empty. Found by holding it at 0 with --poke and watching the bar
// empty (PROGRESS.md).
constexpr uint32_t kGauge = 0x11F;
constexpr uint8_t kGaugeFull = 0x90;

// The round's clock, a byte the game counts down and draws just as it reads: 0x5A is the "90" on the picture,
// so it is plain and not BCD (0x63 is 99, the longest a round runs). 0 outside a round. Found by reading it
// beside the clock on the picture over a round (--trace) (PROGRESS.md).
constexpr uint32_t kClock = 0xFF8109;
constexpr uint8_t kClockFull = 99;

} // namespace sfa3
