function setup()
    SetEncounterSprite("Assets/encounters/02/minotaur/minotaur.png");
    SetEncounterDescription("The minotaur.")

    AddEncounterAbility("Assets/enemies/wyrm_boss/ability_bite.lua",0,0);
    AddEncounterAbility("Assets/enemies/wyrm_boss/ability_firebreath.lua",1,0);
    AddEncounterAbility("Assets/enemies/wyrm_boss/ability_fire.lua",2,0);
    AddEncounterAbility("Assets/enemies/wyrm_boss/ability_nuke.lua",3,0);


    SetEncounterMapPath("Assets/Encounters/02/map.lua");
    SetEncounterDifficulty(DIFFICULTY_MEDIUM);
end