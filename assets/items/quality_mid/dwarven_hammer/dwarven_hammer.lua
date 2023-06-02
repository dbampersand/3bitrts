local cleavePercent = 0.25

function setup(obj)
    SetItemName(obj,"Dwarven Hammer")
    SetItemDescription(obj,"Adds " .. cleavePercent * 100 .. "% cleave.")
    SetItemGoldCost(obj,80)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/dwarven_hammer/dwarven_hammer.png");

end

function attached(obj)
    local f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT
    f1["type"] = EFFECT_CLEAVE
    f1["value"] = cleavePercent;
    f1["name"] = "Dwarven Hammer"
    
    ApplyEffect(obj,{f1})

end

function unattach(obj)
    CureNamedEffect(obj,"Dwarven Hammer",1)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
