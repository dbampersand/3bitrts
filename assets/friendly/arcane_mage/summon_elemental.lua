elementals = {}


function setup()
    AbilitySetPortrait("assets/friendly/monk/icon_fury.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Summon Elemental\n\nSummons one of three random elementals.")
    SetAbilityTargetHint(HINT_NONE)
    SetCooldown(30);
    SetAbilityRange(50)

    elementals[1] = "assets/friendly/arcane_mage/elemental/water_elemental.lua";
    --elementals[2] = "assets/friendly/arcane_mage/elemental/fire_elemental.lua";
    --elementals[2] = "assets/friendly/arcane_mage/elemental/nature_elemental.lua";

end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/toxic.wav",1.25)
    local toSpawn = math.random(#elementals)

    ob = CreateObject(elementals[toSpawn],GetX(GetObjRef()),GetY(GetObjRef()),GetObjFriendliness(GetObjRef()),2);

   return true; 
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end