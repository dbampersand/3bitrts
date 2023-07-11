local lifetime = 10
local atkChance = 0.5
local cooldown = 0
function setup(obj)
    SetItemName(obj,"Opal Necklace")
    SetItemDescription(obj,"When attacking, sometimes summons a healing fairy for "..lifetime .. " seconds.")
    SetItemGoldCost(obj,180)
    SetItemTier(obj,2)
    SetItemIcon(obj, "assets/items/quality_high/opal_necklace/opal_necklace.png");

end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt)
    cooldown = cooldown - dt
    SetItemHighlight(item,false)

    if (cooldown <= 0) then
        cooldown = 0
        SetItemHighlight(item,true)
    end
end

function onattack(item,obj,target,dt,value)
    local roll = RandRange(0,1)


    if (atkChance >= roll and cooldown <= 0) then
        local newObj = CreateObject("assets/summoned/fairy/fairy.lua",GetX(GetObjRef()),GetY(GetObjRef()),TYPE_FRIENDLY,0)
        SetLifetime(newObj,lifetime)
        cooldown = lifetime
    end
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
