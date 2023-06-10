local dodgeChance = 8

function setup(obj)
    SetItemName(obj,"Illusionary Cloak")
    SetItemDescription(obj,"Adds +" .. dodgeChance .. "% dodge chance.")
    SetItemGoldCost(obj,20)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/illusionary_cloak/illusionary_cloak.png");

    
end

function attached(obj)
    local f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_DODGE_CHANCE
    f1["value"] = dodgeChance;  
    f1["name"] = "Illusionary Cloak"

    ApplyEffect(obj,{f1});

end
function unattach(obj)
    CureNamedEffect(obj,"Illusionary Cloak",1)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
