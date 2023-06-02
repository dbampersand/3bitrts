local cooldown = 4
local timer = 0

local active = true

function setup(obj)
    SetItemName(obj,"Managuard")
    SetItemDescription(obj,"Blocks a single source of damage every " .. cooldown .. " seconds")
    SetItemGoldCost(obj,180)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/managuard/managuard.png");
end

function attached(obj)
end

function unattach(obj)
end

function update(dt)
    if (active == false) then
        timer = timer + dt
        if (timer > cooldown) then
            timer = 0
            active = true 
        end
    end
end

function onattack(item,obj,target,dt,value)
end
function onmapchange(obj)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function ondamaged(item,sourceObj,damagedObj,value)
    if (active) then
        active = false
        return 0
    end
    return value
end