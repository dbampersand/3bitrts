local shieldValue = 100

local cooldown = 10

local timer = 0 

function setup(obj)
    SetItemName(obj,"Ring of Shielding")
    SetItemDescription(obj,"Projects a defensive shield every ".. cooldown .. " seconds.")
    SetItemGoldCost(obj,80)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/ring_of_shielding/ring_of_shielding.png");

end

function attached(obj)
end

function update(dt,itemIndex,obj)
    timer = timer + dt

    if (timer > cooldown) then
        timer = 0

        local f1 = {};
        f1["trigger"] = TRIGGER_INSTANT;
        f1["type"] = EFFECT_SHIELD
        f1["value"] = shieldValue
        f1["duration"] = cooldown - 0.5

    
        ApplyEffect(GetObjRef(),{f1})
    
    end
end

function OnAttack(item,obj,target,dt,value)
end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
