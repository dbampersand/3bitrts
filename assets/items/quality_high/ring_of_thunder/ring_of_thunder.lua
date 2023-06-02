local cooldownRate = 0.125

function setup(obj)
    SetItemName(obj,"Ring of Thunder")
    SetItemDescription(obj,"Decreases ability cooldowns.")
    SetItemGoldCost(obj,135)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/ring_of_thunder/ring_of_thunder.png");
end

function attached(obj)
    local f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_COOLDOWN_RATE
    f1["value"] = cooldownRate;  
    f1["name"] = "Ring of Thunder"

    ApplyEffect(obj,{f1});
end

function unattach(obj)
    CureNamedEffect(obj,"Ring of Thunder",1)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end


function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function ondamaged(item,sourceObj,damagedObj,value)
end