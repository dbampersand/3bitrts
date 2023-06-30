local armour = -5

local duration = 12

function setup()
    SetAbilityRange(64) 
    SetCooldown(12);
    AbilitySetCastType(ABILITY_TARGET_ENEMY);

    AbilitySetPortrait("assets/summoned/woven_demon/ability_curse.png");
    SetDescription("[b]Curse\n\nCurses the target, reducing their armour.");
    SetAbilityName("Curse"); 

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/summoned/woven_demon/audio/curse.wav",0.5)

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_ARMOR
    f1["duration"] = duration
    f1["value"] = armour;

    ApplyEffect(obj,{f1})


    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
end
function onchanneled() 
end