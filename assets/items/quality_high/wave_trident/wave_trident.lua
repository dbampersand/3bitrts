local radius = 40
local damage = 15

local tickRate = 0.5
local duration = 2

function setup(obj)
    SetItemName(obj,"Wave Trident")
    SetItemDescription(obj,"Attacks create a cone of projectiles which deal damage over time.")
    SetItemGoldCost(obj,140)
    SetItemTier(obj,2)
    SetItemIcon(obj, "assets/items/quality_high/wave_trident/wave_trident.png");

end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER
    f1["type"] = EFFECT_HURT
    f1["value"] = damage * tickRate
    f1["duration"] = duration
    f1["triggersPerSecond"] = 1/tickRate
    f1["name"] = "Wave Trident"

    CreateConeProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),GetX(target),GetY(target),"",ATTACK_PROJECTILE_ANGLE,80,999,true,ATTACK_HITS_ENEMIES,3,COLOR_DAMAGE,radius,{f1});
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
