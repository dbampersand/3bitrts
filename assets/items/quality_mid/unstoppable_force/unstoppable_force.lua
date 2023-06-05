local dps = 100
local tickRate = 0.5
function setup(obj)
    SetItemName(obj,"Unstoppable Force")
    SetItemDescription(obj,"Allows the unit to push others and deals damage when very close.")
    SetItemGoldCost(obj,70)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/unstoppable_force/unstoppable_force.png");
end

function onmapchange(obj)
    SetObjectPush(true)

    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickRate;  

    local radius = GetWidth(g) / 2

    aoe = CreateAOE(GetX(obj),GetY(obj),"", radius, tickRate, 999, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_HALF, false, obj, {f1})

end

function attached(obj)
    SetObjectPush(true)
end

function update(dt)
    MoveAttack(aoe,GetX(GetObjRef()),GetY(GetObjRef()))
    SetAttackLifetime(aoe,9999)

end


function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
function onabilitycast(item,obj,target,ability,x,y)
end

function beforeabilitycast(item,obj,target,ability,x,y) 
end