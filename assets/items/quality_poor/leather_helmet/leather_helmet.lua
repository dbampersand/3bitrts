
function setup(obj)
    SetItemName(obj,"Leather Helmet")
    SetItemDescription(obj,"+1 defence.")
    SetItemGoldCost(obj,4)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/leather_helmet/leather_helmet.png");

    
end

function Attached(item,obj)
    AddArmor(obj,1)
end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)
end


function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
