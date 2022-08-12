function setup()
    SetEncounterSprite("assets/enemies/wyrm_boss/wyrm.png");
    SetEncounterDescription("Test.")
    SetEncounterName("Test");

    AddEncounterAbility("assets/enemies/wyrm_boss/ability_bite.lua",0,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_firebreath.lua",1,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_fire.lua",2,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_nuke.lua",3,0);


    SetEncounterMapPath("assets/encounters/03/map_1.lua");
    SetEncounterDifficulty(DIFFICULTY_EASY);
    SetEncounterMusic("assets/audio/first_boss.wav");
    EncounterSetNumUnitsToSelect(2);

    SetEncounterLoadScreen("assets/encounters/03/loadscreen.png")
end