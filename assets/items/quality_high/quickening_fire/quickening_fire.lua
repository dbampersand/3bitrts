local increase = 0.75
local damage = 25

function setup(obj)
    SetItemName(obj,"Quickening Fire")
    SetItemDescription(obj,"Deals damage to yourself when attacking, but increases attack damage by ".. increase .. "%.")
    SetItemGoldCost(obj,125)
    SetItemTier(obj,2)
    SetItemIcon(obj, "assets/items/quality_high/quickening_fire/quickening_fire.png");

end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt)
    MoveAttack(aoe,GetX(GetObjRef()),GetY(GetObjRef()))
    SetAttackLifetime(aoe,9999)
end

function onattack(item,obj,target,dt,value)
    value = value + (value * increase)
    HurtObj(obj,damage)
    return value
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
