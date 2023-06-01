local percentCleave = 0.4

function setup(obj)
    SetItemName(obj,"Dark Lash")
    SetItemDescription(obj,"Causes all direct cast abilities to cleave at " .. percentCleave*100 .. "% ")
    SetItemGoldCost(obj,100)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/barbed_wire/barbed_wire.png");
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

end