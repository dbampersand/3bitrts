
function setup()
    AbilitySetPortrait("assets/friendly/ranger/icon_flurry.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Flurry\n\nIncreases attack speed.")
    SetAbilityTargetHint(HINT_NONE)
    SetCooldown(99999999);
    SetCooldownTimer(99999999);
    SetAbilityMaxStacks(5);
    SetAbilityStacks(0)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/toxic.wav",1.25)

    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_ATTACKSPEED
    f1["value"] = -0.15;
    f1["maxStacks"] = 5;
    f1["canStack"] = true;  
    f1["duration"] = 10
    f1["name"] = "Flurry"

    ApplyEffect(GetObjRef(),{f1});
   return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end