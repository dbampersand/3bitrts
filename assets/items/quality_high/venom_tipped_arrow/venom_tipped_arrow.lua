local maxStacks = 6
local dps = 2.5
function setup(obj)
    SetItemName(obj,"Venom-tipped Arrow")
    SetItemDescription(obj,"Applies a poison when attacking that stacks " .. maxStacks .. " times.")
    SetItemGoldCost(obj,120)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/venom_tipped_arrow/venom_tipped_arrow.png");
end

function attached(obj)

end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)
    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps;  
    f1["duration"] = 5
    f1["triggersPerSecond"] = 1
    f1["canStack"]= true;
    f1["maxStacks"] = maxStacks

    ApplyEffect(target,{f1});

end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function OnDamaged(item,sourceObj,damagedObj,value)
end