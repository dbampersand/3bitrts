 function setup()
    SetEncounterSprite("assets/enemies/kobold_king/kobold_king.png");
    SetEncounterDescription("A faction of goblins defend the entrance to the mountain.")
    SetEncounterName("Kobold King");

    AddEncounterAbility("assets/enemies/kobold_king/ability_throw_bot.lua",0,0);
    AddEncounterAbility("assets/enemies/kobold_king/ability_fan_of_knives.lua",1,0);
    AddEncounterAbility("assets/enemies/kobold_king/ability_slash.lua",2,0);
    AddEncounterAbility("assets/enemies/kobold_king/ability_throw_concotion.lua",3,0);
    AddEncounterAbility("assets/enemies/kobold_king/ability_poisoned_spear.lua",4,0);
    AddEncounterAbility("assets/enemies/kobold_king/ability_call_guards.lua",5,0);
    AddEncounterAbility("assets/enemies/kobold_king/ability_cloak_of_fire.lua",6,0);



    SetEncounterMapPath("assets/encounters/0/map_1.lua");
    SetEncounterDifficulty(DIFFICULTY_EASY);
    EncounterSetNumUnitsToSelect(3);

    SetEncounterLoadScreen("assets/encounters/0/loadscreen.png")
    StartsUnlocked(true);
end