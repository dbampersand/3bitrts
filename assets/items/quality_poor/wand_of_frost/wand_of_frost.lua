local duration = 2
local speed = -70

function setup(obj)
    SetItemName(obj,"Wand of Frost")
    SetItemDescription(obj,"Slows enemies when attacked.")
    SetItemGoldCost(obj,20)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/wand_of_frost/wand_of_frost.png");

    
end

function attached(obj)

end
function unattach(obj)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_SPEED
    f1["value"] = speed;
    f1["duration"] = duration  
    f1["name"] = "Wand of Frost"

    ApplyEffect(target,{f1});

end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
