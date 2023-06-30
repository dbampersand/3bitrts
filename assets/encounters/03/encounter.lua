function setup()
    SetEncounterSprite("assets/enemies/cusith/cusith.png");
    SetEncounterDescription("Cu-Sith, the black dog.")
    SetEncounterName("Cu-Sith");

    AddEncounterAbility("assets/enemies/cusith/ability_howl.lua",0,0);
    AddEncounterAbility("assets/enemies/cusith/ability_focus.lua",1,0);
    AddEncounterAbility("assets/enemies/cusith/ability_spin.lua",2,0);
    AddEncounterAbility("assets/enemies/cusith/ability_bite.lua",3,0);
    AddEncounterAbility("assets/enemies/cusith/ability_call_pack.lua",4,0);



    SetEncounterMapPath("assets/encounters/03/map_1.lua");
    SetEncounterDifficulty(DIFFICULTY_MEDIUM);
    EncounterSetNumUnitsToSelect(4);
    SetEncounterLoadScreen("assets/encounters/03/loadscreen.png")
    
end

function mapend()
    return true;
end
function win()

end