
local radius = 20

function setup()
    AbilitySetCastType(ABILITY_POINT); 
    AbilitySetPortrait("assets/friendly/chromamancer/icon_chromophore.png");
    SetAbilityHint(HINT_CIRCLE,radius);
    

    SetAbilityRange(60)
    SetCooldown(30);
    SetDescription("Chromophore\n\nHeals and provides a shield to those in its area.")
end

local aoe = -1;

function casted(x,y,obj,headingx,headingy)

    
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = 20;  
    f1["triggersPerSecond"] = 1
    f1["duration"] = 10

    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_HEAL;
    f2["value"] = 10;  

    local atk = CreateAOE(x,y,"", radius, 1, 4, false, ATTACK_HITS_FRIENDLIES, COLOR_HEAL, DITHER_HORIZONTAL_QUARTER, false, -1, {f1,f2})
    SetAttackCircle(atk,true);
    
    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)

end