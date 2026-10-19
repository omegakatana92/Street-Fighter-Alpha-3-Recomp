// SPDX-License-Identifier: GPL-3.0-only
//
// Street Fighter Alpha 3 (Europe 980904): what the game's own sources add to the port (see PROGRESS.md).
// Nothing here is the game's data: only where the game keeps a thing, and any part of its program that the
// port has to do differently from the board.
//
// The features are put together from the game's own memory (sfa3.h) and the toolkit's fight/ parts
// (fight/fight.h).
#include "game.h"

namespace cps2recomp {

// (the game's own parts; each registers its features and state and is called from around a frame)
void RegisterCheats(); // cheats.cpp
void CheatsFrame(Machine &machine);
void CheatsState(Archive &ar);
void RegisterTraining(); // training.cpp
void TrainingBeforeFrame(Machine &machine);
void TrainingFrame(Machine &machine);
void TrainingState(Archive &ar);

namespace {
    void BeforeFrame(Machine &machine) {
        TrainingBeforeFrame(machine);
    }

    // (the cheats see what a frame did, after it; the training aids show what was just read)
    void Frame(Machine &machine) {
        CheatsFrame(machine);
        TrainingFrame(machine);
    }

    void State(Archive &ar) {
        CheatsState(ar);
        TrainingState(ar);
    }
} // namespace

void RegisterGame() {
    RegisterCheats();
    RegisterTraining();
    TheGame().beforeFrame = BeforeFrame;
    TheGame().frame = Frame;
    TheGame().state = State;
}

} // namespace cps2recomp
