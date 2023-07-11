local aoe = -1
local attackSpeed = -0.15
local tickRate = 1

function setup(obj)
    SetItemName(obj,"Belt of Haste")
    SetItemDescription(obj,"Increases attack speed around the owner.")
    SetItemGoldCost(obj,165)
    SetItemTier(obj,2)
    SetItemIcon(obj, "assets/items/quality_high/belt_of_haste/belt_of_haste.png");

end

function onmapchange(obj)
    local f1 = {}
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ATTACKSPEED;
    f1["value"] = attackSpeed 
    f1["duration"] = tickRate
    f1["overwrites"] = true

    aoe = CreateAOE(GetX(obj),GetY(obj),"", 45, tickRate, 999, false, ATTACK_HITS_FRIENDLIES, COLOR_FRIENDLY_SPEED, DITHER_SPEED_EIGTH, false, obj,0,0,0, {f1})
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
