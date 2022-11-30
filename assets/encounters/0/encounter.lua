function setup()
    SetEncounterSprite("assets/enemies/kobold_king/kobold_king.png");
    SetEncounterDescription("A faction of goblins defend the entrance to the mountain.")
    SetEncounterName("Kobold King");

    AddEncounterAbility("assets/enemies/wyrm_boss/ability_bite.lua",0,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_firebreath.lua",1,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_fire.lua",2,0);
    AddEncounterAbility("assets/enemies/wyrm_boss/ability_nuke.lua",3,0);


    SetEncounterMapPath("assets/encounters/0/map_1.lua");
    SetEncounterDifficulty(DIFFICULTY_EASY);
    EncounterSetNumUnitsToSelect(3);

    SetEncounterLoadScreen("assets/encounters/0/loadscreen.png")
    StartsUnlocked(true);
end