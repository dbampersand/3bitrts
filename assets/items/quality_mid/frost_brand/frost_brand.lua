local time = 3
local attackSpeed = 0.2
local slow = -30


function setup(obj)
    SetItemName(obj,"Frost Brand")
    SetItemDescription(obj,"Slows the attacked target and their attack speed for " .. time)
    SetItemGoldCost(obj,110)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/frost_brand/frost_brand.png");

end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_SPEED
    f1["value"] = speed;  
    f1["duration"] = timer
    f1["overwrites"] = true
    f1["name"] = "Frost Brand Slow"

    local f2 = {};
    f2["trigger"] = TRIGGER_CONST
    f2["type"] = EFFECT_ATTACKSPEED
    f2["value"] = attackSpeed;  
    f2["duration"] = time
    f2["overwrites"] = true
    f2["name"] = "Frost Brand Attack Speed"


    ApplyEffect(target,{f1,f2})
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
