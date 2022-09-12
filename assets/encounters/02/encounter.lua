function setup()
    SetEncounterSprite("assets/encounters/02/minotaur/minotaur.png");
    SetEncounterDescription("The minotaur.")

    AddEncounterAbility("assets/enemies/wyrm_boss/ability_bite.lua",0,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_firebreath.lua",1,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_fire.lua",2,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_nuke.lua",3,0);


    SetEncounterMapPath("assets/encounters/02/map.lua");
    SetEncounterDifficulty(DIFFICULTY_MEDIUM);
    EncounterSetNumUnitsToSelect(4);
    SetEncounterLoadScreen("assets/encounters/02/loadscreen.png")
    
end