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

function OnAttack(item,obj,target,dt,value)
    if (chance >= RandRange(0,1)) then
       value = value * critMultiplier 
    end
    return value;
end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function OnDamaged(item,sourceObj,damagedObj,value)
end