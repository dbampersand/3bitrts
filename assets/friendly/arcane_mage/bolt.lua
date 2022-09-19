
function setup()
    AbilitySetPortrait("assets/friendly/monk/icon_palm.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetDescription("Bolt\n\nDeals damage and reduces cooldown of Mana Sword.");
    SetCooldown(8);
    SetAbilityRange(50);    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/cloak.wav",1.25)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 200;

    ApplyEffect(obj,{f1})

    ModifyAbilityCooldownTimer(GetObjRef(),3,-2);

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end