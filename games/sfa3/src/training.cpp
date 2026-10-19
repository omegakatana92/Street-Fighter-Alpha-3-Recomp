// SPDX-License-Identifier: GPL-3.0-only
//
// Street Fighter Alpha 3: the training aids, put together from the toolkit's fight/ parts (fight/fight.h)
// and the game's own memory (sfa3.h).
//
// The input display needs only what the port read off the panel and the game's facing word, so it is the
// first one; the others the fight/ parts want (frame data, the combo readout, the dummy, the hit boxes) wait
// on more of the game's own memory being known: the players' records are found, with their health, places,
// facing and state picked out, but the state's values are not yet all told apart (PROGRESS.md).
//
// "Training mode" is a session the menu starts and ends (a command each). While one is on, both fighters'
// health and POWER bars are held full and the round's clock stands, so a round can be practised in as long
// as the player likes, and a readout shows each fighter's health and the last hit taken, which the fight/
// parts do not need the game's own state for.
#include "sfa3.h"

#include "fight.h"
#include "machine.h"
#include "state.h"

#include <cstdio>

namespace cps2recomp {

namespace {
    using namespace sfa3;

    fight::InputHistory g_inputs;

    // A training session is on: what the port calls one (the aids that are kFeatureTraining work then).
    bool g_training = false;

    // A round is being fought when both players' health is a sensible amount: outside a round (and in the
    // attract show) the game keeps 0 there, which is what tells the aids not to touch it.
    bool Fighting() {
        for (int n = 0; n < 2; n++) {
            if (Ram16(Player(n) + kHealth) == 0) {
                return false;
            }
        }
        return true;
    }

    // Which way a player faces: the game's own word (sfa3.h), read as "the opponent is to its right". It is
    // only meaningful while a round is fought (outside one the record holds 0), so a player not in a round is
    // taken to face right and the notation is left as it is.
    bool FacingLeft(int n) {
        if (Ram16(Player(0) + kHealth) == 0 && Ram16(Player(1) + kHealth) == 0) {
            return false;
        }
        return Ram16(Player(n) + kFacing) == 0;
    }

    void ReadInputs(Machine &machine) {
        for (int n = 0; n < 2; n++) {
            g_inputs.Push(n, machine.players[n], FacingLeft(n));
        }
    }

    // What the round began with: the full health of each player (learned the first frames it is seen) and the
    // clock the round began at. 0: no round has been seen yet.
    uint16_t healthAtStart[2] = {0, 0};
    uint8_t clockAtStart = 0;

    // The readout's own working: the health each player's record held at the end of the step before, the
    // damage of the last hit taken (0: none yet), the step it landed on, and a count of the steps gone by.
    uint16_t healthSeen[2] = {0, 0};
    int lastHit[2] = {0, 0};
    uint32_t lastHitAt[2] = {0, 0};
    uint32_t steps = 0;

    constexpr int kHitShownFor = 150; // steps a hit stays in the readout (two and a half seconds)

    void ForgetRound() {
        healthAtStart[0] = healthAtStart[1] = 0;
        clockAtStart = 0;
        healthSeen[0] = healthSeen[1] = 0;
        lastHit[0] = lastHit[1] = 0;
        lastHitAt[0] = lastHitAt[1] = 0;
    }

    void StartTraining(Machine &) {
        g_training = true;
        g_inputs.Clear();
        ForgetRound();
    }

    void EndTraining(Machine &) {
        g_training = false;
        g_inputs.Clear();
        ForgetRound();
    }

    // Holds the round as it began: both fighters' health and POWER held full, and the clock still.
    void KeepTheRound() {
        for (int n = 0; n < 2; n++) {
            const uint16_t health = Ram16(Player(n) + kHealth);
            if (health > healthAtStart[n]) {
                healthAtStart[n] = health;
            }
            // (what the frame took off, before it is put back, is a hit: the round's own measure of one)
            if (healthSeen[n] > health && healthSeen[n] - health < healthAtStart[n]) {
                lastHit[n] = static_cast<int>(healthSeen[n] - health);
                lastHitAt[n] = steps;
            }
            healthSeen[n] = health;
            if (healthAtStart[n] != 0) {
                SetRam16(Player(n) + kHealth, healthAtStart[n]);
                SetRam16(Player(n) + kHealthShown, healthAtStart[n]);
            }
            SetRam8(Player(n) + kGauge, kGaugeFull);
        }
        if (clockAtStart == 0) {
            clockAtStart = Ram8(kClock);
        }
        if (clockAtStart != 0) {
            SetRam8(kClock, clockAtStart);
        }
    }

    void Readout(std::vector<HudLine> &lines) {
        for (int n = 0; n < 2; n++) {
            const unsigned health = healthAtStart[n] != 0 ? healthAtStart[n] : Ram16(Player(n) + kHealth);
            char text[64];
            if (lastHit[n] != 0 && steps - lastHitAt[n] < kHitShownFor) {
                snprintf(text, sizeof(text), "P%d  health %u  (hit -%d)", n + 1, health, lastHit[n]);
            } else {
                snprintf(text, sizeof(text), "P%d  health %u", n + 1, health);
            }
            lines.push_back({n == 0 ? 0.02f : 0.98f, 0.075f, n == 0 ? -1 : 1, text});
        }
    }
} // namespace

void TrainingBeforeFrame(Machine &machine) {
    ReadInputs(machine);
}

void TrainingFrame(Machine &) {
    steps++;
    std::vector<HudLine> lines;
    if (g_training) {
        lines.push_back({0.5f, 0.02f, 0, "TRAINING"});
        if (!Fighting()) {
            ForgetRound();
        } else {
            KeepTheRound();
        }
        Readout(lines);
    }
    if (FeatureOn("input_display")) {
        g_inputs.Hud(0, lines);
        g_inputs.Hud(1, lines);
    }
    SetHud(lines);
}

void TrainingState(Archive &ar) {
    ar.Pod(g_training);
    ar.Pod(healthAtStart[0]);
    ar.Pod(healthAtStart[1]);
    ar.Pod(clockAtStart);
    // (the input display's own frame counts are not part of the save state: a load starts the display over)
    if (!ar.Saving()) {
        g_inputs.Clear();
        ForgetRound();
    }
}

// Whether a training session is on (runtime/game.h: the aids that are kFeatureTraining work only then)
bool TrainingOn() {
    return g_training;
}

void RegisterTraining() {
    Game &game = TheGame();
    game.features.push_back({"input_display", "Training", "Input display",
                             "What each player holds, and for how long, at their side of the picture."});
    game.commands.push_back({"training_mode", "Training", "Training mode",
                             "Both fighters' health and POWER bars are held full, the round's clock stands, "
                             "and the health and the last hit are read out beside the picture, so a round can "
                             "be practised in as long as you like.",
                             StartTraining, kKeyNone});
    game.commands.push_back(
        {"end_training", "Training", "End training", "The round is left to run as it is again.", EndTraining, kKeyNone});
    game.inTraining = TrainingOn;
}

} // namespace cps2recomp
