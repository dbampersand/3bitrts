function setup()
    --SetEncounterSprite();
    --SetEncounterDescription("Tutorial.")
    SetEncounterName("Tutorial");

    SetEncounterMapPath("assets/encounters/tutorial/map.lua");
    SetEncounterDifficulty(DIFFICULTY_EASY);

    SetEncounterLoadScreen("assets/encounters/01/loadscreen.png")

    RemoveFromEncountersList();
end