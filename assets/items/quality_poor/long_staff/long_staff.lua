local cooldown = 8

local timer = 8

local speedDuration = 3
local speedDecrease = -0.8

function setup(obj)
    SetItemName(obj,"Long Staff")
    SetItemDescription(obj,"When damaged, pushes the attacking object away with a cooldown of " .. cooldown .. " seconds and slows them.")
    SetItemGoldCost(obj,20)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/long_staff/long_staff.png");
end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt,item)
    timer = timer + dt
    if (timer >= cooldown) then
        timer = cooldown
        SetItemHighlight(item,true)
    else
        SetItemHighlight(item,false)
    end
end
function ondamaged(item,sourceObj,damagedObj,value, isFromEffect)
    if (isFromEffect == false and timer >= cooldown) then
        PushObj(GetX(damagedObj),GetY(damagedObj),sourceObj,600,0.1)

        local f1 = {};
        f1["trigger"] = TRIGGER_CONST
        f1["type"] = EFFECT_SPEED
        f1["value"] = GetSpeed(sourceObj) * speedDecrease;  
        f1["duration"] = speedDuration
        f1["overwrites"] = true
        f1["name"] = "Long Staff"
        ApplyEffect(sourceObj,{f1})

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