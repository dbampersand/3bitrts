function setup()
    SetEncounterSprite("assets/enemies/wyrm_boss/wyrm.png");
    SetEncounterDescription("The wyrm.")
    SetEncounterName("Wyrm");

    AddEncounterAbility("assets/enemies/wyrm_boss/ability_bite.lua",0,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_firebreath.lua",1,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_fire.lua",2,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_nuke.lua",3,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_firebreath.lua",4,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_summon_adds.lua",5,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_bomb.lua",6,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_rage.lua",7,0);


    SetEncounterMapPath("assets/encounters/01/map_2.lua");
    SetEncounterDifficulty(DIFFICULTY_EASY);
    EncounterSetNumUnitsToSelect(3);

    SetEncounterLoadScreen("assets/encounters/01/loadscreen.png")
    StartsUnlocked(true);
end 