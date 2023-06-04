local percent = 0.1
function setup(obj)
    SetItemName(obj,"Helping Hand")
    SetItemDescription(obj,"When attacking, creates a shield for " .. percent*100 .. "% of the damage done.")
    SetItemGoldCost(obj,30)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/helping_hand/helping_hand.png");

    
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    local shield = value * percent
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = shield;
    f1["duration"] = 10;
    
    ApplyEffect(obj,{f1})
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
