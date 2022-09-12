function setup()
    AbilitySetCastType(ABILITY_TOGGLE);

    SetAbilityRange(20)
    SetCooldown(10);
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_heal.png");
    SetDescription("Song of Healing\n\nProvides healing in a radius.")

end

local aoe = -1;
local g = 5;
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/bard/audio/song_of_healing.wav",1)

    UntoggleOthers();

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    --f1["numTriggers"] = 5
    f1["value"] = 10;  
    f1["duration"] = 1.5;
    f1["triggersPerSecond"] = 1
    f1["name"] = "Song of Healing"


    aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_HALF,false,-1, {f1})
    SetAttackCircle(aoe,true);
    return true; 
end

function untoggle()
    RemoveAttack(aoe);
    aoe = -1;
    return true;
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
    MoveAttack(GetAttackRef(),GetX(obj),GetY(obj));

end