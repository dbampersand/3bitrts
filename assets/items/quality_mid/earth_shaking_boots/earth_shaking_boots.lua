local aoe = -1

local tickRate = 0.5
local dps = 30

function setup(obj)
    SetItemName(obj,"Earth Shaking Boots")
    SetItemDescription(obj,"Deals damage around the owner.")
    SetItemGoldCost(obj,85)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/earth_shaking_boots/earth_shaking_boots.png");

end

function onmapchange(obj)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickRate;  

    aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, tickRate, 999, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_HALF, false, obj, {f1})
end

function attached(obj)

end

function update(dt)
    MoveAttack(aoe,GetX(GetObjRef()),GetY(GetObjRef()))
    SetAttackLifetime(aoe,9999)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
