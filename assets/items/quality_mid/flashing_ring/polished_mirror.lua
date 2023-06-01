local seconds = 3
local missChance = 10


function setup(obj)
    SetItemName(obj,"Polished Mirror")
    SetItemDescription(obj,"Blinds the attacked target for " .. seconds .. ", causing " .. missChance .. "% miss chance.")
    SetItemGoldCost(obj,110)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/polished_mirror/polished_mirror.png");

end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_MISS_CHANCE
    f1["value"] = missChance;  
    f1["duration"] = seconds

    ApplyEffect(target,{f1})
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
