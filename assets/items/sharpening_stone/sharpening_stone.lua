
function setup(obj)
    SetItemName(obj,"Sharpening Stone")
    SetItemDescription(obj,"+1 Attack")
    SetItemGoldCost(obj,4)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/sharpening_stone/sharpening_stone.png");

    
end

function Attached(item,obj)

end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)
    local v = value + 2
end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
