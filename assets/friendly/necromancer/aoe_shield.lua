local speed = 50;
local duration = 2;
local aoe = -1


function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_shield.png");
    AbilitySetCastType(ABILITY_TOGGLE);
    SetAbilityRange(duration * speed)
    SetCooldown(10)
    SetDescription("Song of Shielding\n\nProvides shielding in a radius.")
    
end
function casted()
    PlaySound("assets/friendly/bard/audio/song_of_shield.wav",1)

    UntoggleOthers();
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    --f1["numTriggers"] = 5
    f1["value"] = 10;
    f1["duration"] = 1.5;
    f1["triggersPerSecond"] = 1
    f1["name"] = "Song of Shielding"
    f1["overwrites"] = true;

    aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_SHIELD,DITHER_STAR_HALF, false, -1, {f1})
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

function abilitytick(x, y, durationLeft)
    MoveAttack(GetAttackRef(),GetX(obj),GetY(obj));
    
end