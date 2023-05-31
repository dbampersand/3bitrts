local moveSpeed = 20

function setup(obj)
    SetItemName(obj,"Hermes' Boots")
    SetItemDescription(obj,"+" .. moveSpeed .. " movement speed")
    SetItemGoldCost(obj,50)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/hermes_boots/hermes_boots.png");

end

function attached(obj)
    local f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_SPEED
    f1["value"] = moveSpeed
    f1["name"] = "Hermes' Boots"

    ApplyEffect(obj,{f1})
end

function unattach(obj)
    CureNamedEffect(obj,"Hermes' Boots",1)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
