
function setup()
    AbilitySetPortrait("assets/friendly/ranger/icon_flurry.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetDescription("Fury\n\nA devastating single target attack. Uses all stacks of chi to deal proportional damage.")
    SetAbilityTargetHint(HINT_NONE)
    SetCooldown(99999999);
    SetCooldownTimer(99999999);
    SetAbilityMaxStacks(5);
    SetAbilityStacks(0)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/toxic.wav",1.25)
    
    f1 = {}
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 250 * GetAbilityStacks(GetObjRef(),GetAbilityRef());

    ApplyEffect(obj,{f1});

    AddStack(GetObjRef(),2,-999);
    AddStack(GetObjRef(),3,-999);
    AddStack(GetObjRef(),4,-999);

   return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end