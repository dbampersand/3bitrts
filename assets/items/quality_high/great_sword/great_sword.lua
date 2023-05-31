local critMultiplier = 3
local chance = 0.1

function setup(obj)
    SetItemName(obj,"Great Sword")
    SetItemDescription(obj,"When attacking, sometimes deal " .. critMultiplier .. " times critical damage.")
    SetItemGoldCost(obj,120)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/great_sword/great_sword.png");
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    if (chance >= RandRange(0,1)) then
       value = value * critMultiplier 
    end
    return value;
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function ondamaged(item,sourceObj,damagedObj,value)
end