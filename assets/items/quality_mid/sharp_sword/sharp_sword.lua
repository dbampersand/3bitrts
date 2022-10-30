
function setup(obj)
    SetItemName(obj,"Sharp Sword")
    SetItemDescription(obj,"+2 Attack")
    SetItemGoldCost(obj,10)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/sharp_sword/sharp_sword.png");

end

function attached(obj)

end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)
    local v = value + 2
end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
