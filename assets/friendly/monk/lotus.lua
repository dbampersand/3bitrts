
function setup()
    AbilitySetPortrait("assets/friendly/monk/icon_lotus.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Lotus\n\nCauses a lotus to bloom, healing yourself. Uses a chi charge.")
    SetCooldown(0);
    SetAbilityRange(50);

    SetCooldown(99999999);
    SetCooldownTimer(99999999);
    SetAbilityMaxStacks(5);
    SetAbilityStacks(0)

end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/summon.wav",1.25)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    f1["value"] = 100;
    ApplyEffect(GetObjRef(),{f1})

    AddStack(GetObjRef(),2,-1);
    AddStack(GetObjRef(),3,-1);



   return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end