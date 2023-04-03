
function setup()
    AbilitySetPortrait("assets/friendly/arcane_mage/mana_sword/icon_slash.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetDescription("Slash\n\nSwings the sword, dealing damage. Generates mana.");
    SetCooldown(4);
    SetAbilityRange(50);   
    SetManaCost(0); 
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/cloak.wav",1.25)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 25;

    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT
    f2["type"] = EFFECT_ADD_MANA
    f2["value"] = 20;




    ApplyEffect(obj,{f1})
    ApplyEffect(GetObjRef(),{f2})

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end