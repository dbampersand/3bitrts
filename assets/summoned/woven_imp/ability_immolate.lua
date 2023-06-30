local shield = 120
local AS = -0.1
local duration = 12

function setup()
    SetAbilityRange(64) 
    SetCooldown(32);
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);

    AbilitySetPortrait("assets/summoned/woven_imp/ability_immolate.png");
    SetDescription("[b]Immolate\n\nGives the target a shield and increases their attack speed.");
    SetAbilityName("Immolate"); 

end

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/summoned/woven_imp/audio/immolate.wav",0.5)


    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_SHIELD
    f1["value"] = shield;
    f1["duration"] = duration;

    local f2 = {};
    f2["trigger"] = TRIGGER_CONST
    f2["type"] = EFFECT_ATTACKSPEED
    f2["value"] = AS;
    f2["duration"] = duration;


    ApplyEffect(obj,{f1,f2})


    

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