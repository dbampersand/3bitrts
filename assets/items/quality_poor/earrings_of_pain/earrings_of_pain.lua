local tickRate = 1
local dps = 15
function setup(obj)
    SetItemName(obj,"Earrings of Pain")
    SetItemDescription(obj,"Attacks also cause a damage over time effect.")
    SetItemGoldCost(obj,20)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/earrings_of_pain/earrings_of_pain.png");
end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    local f1 = {}
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickRate;  
    f1["overwrites"] = true
    f1["name"] = "Earrings of Pain"
    f1["triggersPerSecond"] = 1 / tickRate
    f1["duration"] = 10

    ApplyEffect(target,{f1})
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
