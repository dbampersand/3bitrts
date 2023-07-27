function setup()
    SetEncounterSprite("assets/enemies/cusith/cusith.png");
    SetEncounterDescription("A huge sea serpent. It's eyes glow with a terrifying flame, ")
    SetEncounterName("Stoor Worm");

    AddEncounterAbility("assets/enemies/cusith/ability_howl.lua",0,0);
    AddEncounterAbility("assets/enemies/cusith/ability_focus.lua",1,0);
    AddEncounterAbility("assets/enemies/cusith/ability_spin.lua",2,0);
    AddEncounterAbility("assets/enemies/cusith/ability_bite.lua",3,0);
    AddEncounterAbility("assets/enemies/cusith/ability_call_pack.lua",4,0);
    AddEncounterAbility("assets/enemies/cusith/ability_crush.lua",5,0);
    AddEncounterAbility("assets/enemies/cusith/ability_fangs.lua",6,0);




    SetEncounterMapPath("assets/encounters/04/map_final.lua");
    SetEncounterDifficulty(DIFFICULTY_MEDIUM);
    EncounterSetNumUnitsToSelect(4);
    SetEncounterLoadScreen("assets/encounters/04/loadscreen.png")
    SetEncounterSelectScreenSprite("assets/encounters/04/levelselectsprite.png");
    
end

function mapend()
    return true;
end
function win()

end