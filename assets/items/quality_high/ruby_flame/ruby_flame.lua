local lifetime = 10
local atkChance = 0.5

local cooldown = 0

function setup(obj)
    SetItemName(obj,"Ruby Flame")
    SetItemDescription(obj,"When attacking, sometimes summons a ruby star for "..lifetime .. " seconds.")
    SetItemGoldCost(obj,180)
    SetItemTier(obj,2)
    SetItemIcon(obj, "assets/items/quality_high/ruby_flame/ruby_flame.png");

end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt)
    cooldown = cooldown - dt

    if (cooldown < 0) then
        cooldown = 0
    end
end

function onattack(item,obj,target,dt,value)
    local roll = RandRange(0,1)
    if (atkChance >= roll and cooldown <= 0) then
        local newObj = CreateObject("assets/summoned/ruby_star/ruby_star.lua",GetX(GetObjRef()),GetY(GetObjRef()),TYPE_FRIENDLY,0)
        SetLifetime(newObj,lifetime)

        cooldown = lifetime
    end
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
