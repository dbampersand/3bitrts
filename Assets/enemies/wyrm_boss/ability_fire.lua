function setup()
    SetAbilityRange(2)
    SetAbilityCooldown(10)
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_fire.png");
    SetDescription("[b]Fire\n\nLights the ground on fire, dealing damage in a aoe.")

end

function casted(x,y,obj,headingx,headingy)

    f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["triggersPerSecond"] = 5
    f1["value"] = 2;  
    f1["duration"] = 1;
    f1["portrait"] = "assets/enemies/wyrm_boss/ability_fire_effect_portrait"

    CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 10, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HALF, {f1})
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end