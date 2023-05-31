
function setup(obj)
    SetItemName(obj,"Leather Helmet")
    SetItemDescription(obj,"+1 defence.")
    SetItemGoldCost(obj,20)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/leather_helmet/leather_helmet.png");

    
end

function attached(obj)
    AddArmor(obj,1)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end


function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
