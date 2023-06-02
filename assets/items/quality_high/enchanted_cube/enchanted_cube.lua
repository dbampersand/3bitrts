local heal = 0.20

function setup(obj)
    SetItemName(obj,"Enchanted Cube")
    SetItemDescription(obj,"When attacked, heal for " .. heal * 100 .. "% of the damage.")
    SetItemGoldCost(obj,90)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/enchanted_cube/enchanted_cube.png");
end

function attached(obj)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function ondamaged(item,sourceObj,damagedObj,value)
    Heal(damagedObj,value*heal)
end
function onmapchange(obj)
end