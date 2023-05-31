local aoe = -1
local attackSpeed = -0.15

function setup(obj)
    SetItemName(obj,"Belt of Haste")
    SetItemDescription(obj,"Increases attack speed around the owner.")
    SetItemGoldCost(obj,165)
    SetItemTier(obj,2)
    SetItemIcon(obj, "assets/items/quality_high/belt_of_haste/belt_of_haste.png");

end

function onmapchange(obj)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_ATTACKSPEED;
    f1["value"] = attackSpeed 

    aoe = CreateAOE(GetX(obj),GetY(obj),"", 45, tickRate, 999, false, ATTACK_HITS_ENEMIES, COLOR_SPEED, DITHER_SPEED_QUARTER, false, obj, {f1})
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
