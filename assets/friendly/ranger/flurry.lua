
function setup()
    AbilitySetPortrait("assets/friendly/ranger/icon_flurry.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Flurry\n\nIncreases attack speed and damage.")
    SetAbilityHint(HINT_NONE)
    SetCooldown(99999999);
    SetCooldownTimer(99999999);
    SetAbilityMaxStacks(5);
    SetAbilityStacks(0)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/ranger/audio/flurry.wav",1.25)
    local f1 = {}
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_ATTACKSPEED
    f1["value"] = -0.09;
    f1["maxStacks"] = 5;
    f1["canStack"] = true;  
    f1["duration"] = 10
    f1["name"] = "Flurry"

    local f2 = {}
    f2["trigger"] = TRIGGER_CONST
    f2["type"] = EFFECT_ATTACKDAMAGE
    f2["value"] = 4;
    f2["maxStacks"] = 5;
    f2["canStack"] = true;  
    f2["duration"] = 10
    f2["name"] = "Flurry Damage"




    ApplyEffect(GetObjRef(),{f1,f2});
   return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end