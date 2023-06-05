local abilityPotency = 0.08
local maxStacks = 10
local objAttacking = -1
local duration = 20
local numStacksAdded = 0

function setup(obj)
    SetItemName(obj,"Spellshard")
    SetItemDescription(obj,"Increases ability potency by " .. abilityPotency*100 .. "% for each attack on the same target. Stacks "..maxStacks.." times")
    SetItemGoldCost(obj,180)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/spellshard/spellshard.png");
end

function attached(obj)
end

function unattach(obj)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    if (objAttacking ~= target or objAttacking == -1) then
        CureNamedEffect(obj,"Spellshard",maxStacks)
        objAttacking = target
        numStacks = 0
    end

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ABILITY_POTENCY
    f1["value"] = abilityPotency;  
    f1["maxStacks"] = maxStacks
    f1["canStack"] = true
    f1["name"] = "Spellshard"
    f1["duration"] = duration

    ApplyEffect(obj,{f1});

    numStacks = numStacks + 1

    SetItemStackString(item,string.format("%.0f", numStacks))



end
function onmapchange(obj)
    objAttacking = -1
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function ondamaged(item,sourceObj,damagedObj,value)
end