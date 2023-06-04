--200% at 0% hp
local total = 2

function setup(obj)
    SetItemName(obj,"Beating Heart")
    SetItemDescription(obj,"Adds extra damage proportional to how much health percent is missing.")
    SetItemGoldCost(obj,140)
    SetItemTier(obj,2)
    SetItemIcon(obj, "assets/items/quality_high/beating_heart/beating_heart.png");

end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    local add = (1 - (GetHPPercent(GetObjRef()) / 100)) * total

    value = value + (value * add)

    return value
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
