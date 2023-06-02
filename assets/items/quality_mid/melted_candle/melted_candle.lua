local armorReduce = -2
local duration = 6

function setup(obj)
    SetItemName(obj,"Melted Candle")
    SetItemDescription(obj,"When attacking, removes " .. armorReduce .. " armor for " .. duration .. " seconds")
    SetItemGoldCost(obj,90)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/melted_candle/melted_candle.png");

end

function attached(obj)

end

function unattach(obj)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    local f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_ARMOR
    f1["value"] = armorReduce;
    f1["name"] = "Melted Candle"
    f1["duration"] = duration
    
    ApplyEffect(target,{f1})

end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
