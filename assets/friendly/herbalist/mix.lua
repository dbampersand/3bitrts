local efficiency = 0.6

function setup()
    AbilitySetPortrait("assets/friendly/herbalist/icon_mix.png");
    SetDescription("Mix\n\nConvert all healing over time effects into an instant heal at " .. efficiency * 100 .. "% efficiency.")

    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);

    SetCooldown(4);
    SetManaCost(20);

end


function casted(x,y,obj,headingx,headingy)
    local effects = GetEffects(obj);

    local totalHeal = 0
    for i = 1, #effects do
        if (effects[i].trigger == TRIGGER_TIMER and effects[i].effectType == EFFECT_HEAL) then
            local value = effects[i].value;
            local stacks = effects[i].stacks;
            local triggers = effects[i].triggersPerSecond;
            local duration = effects[i].durationLeft;

            totalHeal = totalHeal + ((value * stacks)/triggers)*duration

            CureNamedEffect(obj,effects[i].name,effects[i].stacks);
        end
    end

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HEAL
    f1["value"] = totalHeal * efficiency;
    ApplyEffect(obj,{f1});
    return true; 
end

function untoggle()
    KillObj(totem);
    totem = -1;
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
end