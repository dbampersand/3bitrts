local radius = 30
local aoe = -1;

function setup()
    AbilitySetCastType(ABILITY_TOGGLE);
    AbilitySetHintColor(EFFECT_HEAL)

    SetAbilityRange(20)
    SetCooldown(10);
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_heal.png");
    SetDescription("Fallain\n\nProvides healing in a radius.")

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/bard/audio/aoe_heal.wav",1)

    UntoggleOthers();

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    --f1["numTriggers"] = 5
    f1["value"] = 10;  
    f1["duration"] = 1.5;
    f1["triggersPerSecond"] = 1
    f1["name"] = "Song of Healing"


    aoe = CreateAOE(GetX(obj),GetY(obj),"", radius, 1, 10, true, ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_HEAL,DITHER_ATTACK_DAMAGE_QUARTER,false,-1, 0,0,0,{f1})
   -- SetAttackCircle(aoe,true);
    return true; 
end

function untoggle()
    RemoveAttack(aoe);
    aoe = -1;
    return true;
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    MoveAttack(attackRef,GetX(obj),GetY(obj));

end