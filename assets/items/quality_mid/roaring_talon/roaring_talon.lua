
function setup(obj)
    SetItemName(obj,"Roaring Talon")
    SetItemDescription(obj,"Doubles all threat generated.")
    SetItemGoldCost(obj,75)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/roaring_talon/roaring_talon.png");

end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
    if (effectType == EFFECT_THREAT) then
        return value * 2;
    end
    return valur;
end
