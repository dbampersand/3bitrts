
function setup()
    AbilitySetPortrait("assets/friendly/rogue/icon_toxic.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(120)
    SetDescription("Toxic\n\nConverts all stacks of poison to damage, with more stacks being worth more.")
    SetAbilityTargetHint(HINT_NONE)
    SetCooldown(0);
end
function casted(x,y,obj,headingx,headingy)

    local numStacks = CureNamedEffect(obj,"Poison",999);
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = (numStacks*numStacks) * 20;  
    ApplyEffect(obj,{f1});
   return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end