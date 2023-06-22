
function setup()
    AbilitySetPortrait("assets/friendly/arcane_mage/icon_bolt.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetDescription("Bolt\n\nDeals damage and reduces cooldown of Mana Sword.");
    SetCooldown(6);
    SetAbilityRange(80);    
    SetManaCost(15);
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/arcane_mage/audio/bolt.wav",1)

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