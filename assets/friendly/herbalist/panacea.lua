local totem = -1

function setup()
    AbilitySetPortrait("assets/friendly/herbalist/icon_panacea.png");
    SetDescription("Panacea\n\nCures a bad effect from target and provides a shield. Generates mana when it cures an effect.")

    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);

    SetCooldown(15);
    SetManaCost(10);
end


function casted(x,y,obj,headingx,headingy)
    local before = GetNumEffects(obj);

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_CURE
    f1["value"] = 1
    ApplyEffect(obj,{f1});
    local after = GetNumEffects(obj);

    local f3 = {};
    f3["trigger"] = TRIGGER_INSTANT
    f3["type"] = EFFECT_SHIELD
    f3["value"] = 100
    ApplyEffect(obj,{f3});


    --Add mana if we've cured an effect
    if (before - after ~= 0) then
        local f2 = {};
        f2["trigger"] = TRIGGER_INSTANT
        f2["type"] = EFFECT_ADD_MANA
        f2["value"] = 25
    
        ApplyEffect(GetObjRef(),{f2})
    end

    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
end