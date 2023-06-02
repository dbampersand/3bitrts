local attackSpeed = 0.1
local maxStacks = 10

local objAttacking = -1
local duration = 20


function setup(obj)
    SetItemName(obj,"Necklace of Swarms")
    SetItemDescription(obj,"Increases attack speed by " .. abilityPotency*100 .. "% when attacking the same target, resetting when a different target is attacked. Stacks "..maxStacks.." times")
    SetItemGoldCost(obj,140)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/necklace_of_swarms/necklace_of_swarms.png");
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    if (objAttacking ~= target or objAttacking == -1) then
        CureNamedEffect(obj,"Necklace of Swarms",maxStacks)
        objAttacking = target
    end

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ATTACKSPEED
    f1["value"] = attackSpeed;  
    f1["maxStacks"] = maxStacks
    f1["canStack"] = true
    f1["name"] = "Necklace of Swarms"
    f1["duration"] = duration

    ApplyEffect(obj,{f1});


end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function ondamaged(item,sourceObj,damagedObj,value)
end