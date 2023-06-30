function setup()
    SetEncounterSprite("assets/encounters/02/minotaur/minotaur.png");
    SetEncounterDescription("The minotaur. This beast lords over an entire floor of the mountain on its own, forcing any passers by into a deadly duel to the death.")
    SetEncounterName("Minotaur");

    AddEncounterAbility("assets/enemies/minotaur/ability_smash.lua",0,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_rampage.lua",1,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_quake.lua",2,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_chuck.lua",3,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_charge.lua",4,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_summon.lua",5,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_bellow.lua",6,0);
    AddEncounterAbility("assets/enemies/minotaur/ability_hibernate.lua",7,0);


    SetEncounterMapPath("assets/encounters/02/map.lua");
    SetEncounterDifficulty(DIFFICULTY_MEDIUM);
    EncounterSetNumUnitsToSelect(4);
    SetEncounterLoadScreen("assets/encounters/02/loadscreen.png")
end 

function win()
    UnlockEncounter("assets/encounters/03");
end