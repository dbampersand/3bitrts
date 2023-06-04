local percent = 0.02
local cooldown = 1

local timer = 0

function setup(obj)
    SetItemName(obj,"Endless Lamp")
    SetItemDescription(obj,"Heals the target for " .. percent .. "% of their health every ".. cooldown .. " seconds.")
    SetItemGoldCost(obj,20)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/endless_lamp/endless_lamp.png");
end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt)
    timer = timer + dt
    if (timer > cooldown) then
        local f1 = {};
        f1["trigger"] = TRIGGER_INSTANT
        f1["type"] = EFFECT_HEAL_PERCENT
        f1["value"] = percent
        
        ApplyEffect(GetObjRef(),{f1})

        timer = 0
    
    end
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
function onabilitycast(item,obj,target,ability,x,y)
    
end

function beforeabilitycast(item,obj,target,ability,x,y) 
end