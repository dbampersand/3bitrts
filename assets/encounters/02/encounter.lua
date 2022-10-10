function setup()
    SetEncounterSprite("assets/encounters/02/minotaur/minotaur.png");
    SetEncounterDescription("The minotaur.")

    AddEncounterAbility("assets/enemies/minotaur/ability_smash.lua",0,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_rampage.lua",1,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_quake.lua",2,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_chuck.lua",3,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_charge.lua",4,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_summon.lua",5,0);


    SetEncounterMapPath("assets/encounters/02/map.lua");
    SetEncounterDifficulty(DIFFICULTY_MEDIUM);
    EncounterSetNumUnitsToSelect(4);
    SetEncounterLoadScreen("assets/encounters/02/loadscreen.png")
    
end