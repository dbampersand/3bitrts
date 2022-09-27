
function setup(obj)
    SetItemName(obj,"Sharp Sword")
    SetItemDescription(obj,"+2 Attack")
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
