local lifetime = 20
function setup()
    AbilitySetPortrait("assets/friendly/arcane_mage/elemental/icon_transmute.png");
    AbilitySetCastType(ABILITY_TARGET_ALL);
    SetDescription("Transmute\n\nApplies a heal over time effect if used on allies, or a damage over time effect if used on enemies");
    SetCooldown(60);
    SetAbilityRange(9999);    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/cloak.wav",1.25)


    f1 = {};
    
    if (GetObjFriendliness(obj) == GetObjFriendliness(GetObjRef())) then
        f1["trigger"] = TRIGGER_TIMER
        f1["duration"] = 10
        f1["type"] = EFFECT_HEAL
        f1["value"] = 10
        f1["triggersPerSecond"] = 1

    else
        f1["trigger"] = TRIGGER_TIMER
        f1["duration"] = 10
        f1["type"] = EFFECT_HURT
        f1["value"] = 10
        f1["triggersPerSecond"] = 1    
    end
    
    ApplyEffect(obj,{f1});
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
end