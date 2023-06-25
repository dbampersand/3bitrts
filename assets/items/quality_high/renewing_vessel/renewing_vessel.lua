local hps = 50
local tickRate = 2
local aoe = -1
function setup(obj)
    SetItemName(obj,"Renewing Vessel")
    SetItemDescription(obj,"Heals in an area around the owner.")
    SetItemGoldCost(obj,80)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/renewing_vessel/renewing_vessel.png");
end

function onmapchange(obj)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    f1["value"] = hps * tickRate;  

    aoe = CreateAOE(GetX(obj),GetY(obj),"", 50, tickRate, 999, false, ATTACK_HITS_FRIENDLIES, COLOR_HEAL, DITHER_HEAL_EIGTH, false, obj, {f1})
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
