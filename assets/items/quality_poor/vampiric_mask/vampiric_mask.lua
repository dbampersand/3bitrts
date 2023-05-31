lifestealPercent = 0.2

function setup(obj)
    SetItemName(obj,"Vampiric Mask")
    SetItemDescription(obj,"Grants +"..lifestealPercent*100 .. "% Lifesteal")
    SetItemGoldCost(obj,30)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/vampiric_mask/vampiric_mask.png");

    
end

function attached(obj)
    local f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_LIFESTEAL
    f1["value"] = lifestealPercent
    f1["name"] = "Vampiric Mask"

    ApplyEffect(obj,{f1})

end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)
end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
