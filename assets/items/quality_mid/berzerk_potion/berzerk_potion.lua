local armorValue = -5
local damageValue = 8


function setup(obj)
    SetItemName(obj,"Berzerk Potion")
    SetItemDescription(obj,"Deal more damage on hit, but also take more damage.")
    SetItemGoldCost(obj,60)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/berzerk_potion/berzerk_potion.png");

end

function attached(obj)
    local f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_ARMOR
    f1["value"] = value
    f1["name"] = "Berzerk Potion"

    ApplyEffect(obj,{f1})
end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)
    local v = value + damageValue
    return v;
end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
