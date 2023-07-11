local aoe = -1
local speed = 45
local tickRate = 1
function setup(obj)
    SetItemName(obj,"War Drums")
    SetItemDescription(obj,"Increases move speed around you.")
    SetItemGoldCost(obj,15)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/war_drums/war_drums.png");

    
end

function onmapchange(obj)
    local f1 = {}
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_SPEED;
    f1["value"] = speed;  
    f1["overwrites"] = true
    f1["name"] = "War Drums"
    f1["duration"] = 10

    aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, tickRate, 999, false, ATTACK_HITS_FRIENDLIES, COLOR_FRIENDLY_SPEED, DITHER_SPEED_EIGTH, false, obj,0,0,0, {f1})
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
