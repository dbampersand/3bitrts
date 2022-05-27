#include "gamestate.h"
#include "sound.h"

void StartCombat()
{
    if (combatStarted)
    {
        return;
    }
    PlayEncounterMusic();
    combatStarted = true;
}