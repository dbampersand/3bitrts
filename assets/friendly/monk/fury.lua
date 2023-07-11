
function setup()
    AbilitySetPortrait("assets/friendly/monk/icon_fury.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    AbilitySetHintColor(EFFECT_HURT)
    SetDescription("Fury\n\nA devastating single target attack. Uses all stacks of chi to deal proportional damage.")
    SetCooldown(99999999);
    SetCooldownTimer(99999999);
    SetAbilityMaxStacks(5);
    SetAbilityStacks(0)

    SetAbilityRange(50)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/monk/audio/fury.wav",1)

    local numStacks = GetAbilityStacks(GetObjRef(),GetAbilityRef())
    
    f1 = {}
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 200 * numStacks;

    ApplyEffect(obj,{f1});

    AddStack(GetObjRef(),2,-999);
    AddStack(GetObjRef(),3,-999);
    AddStack(GetObjRef(),4,-999);

    ShakeScreen(numStacks,0.2)

   return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end