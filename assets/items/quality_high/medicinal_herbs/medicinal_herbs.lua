local amt = 5

function setup(obj)
    SetItemName(obj,"Medicinal Herbs")
    SetItemDescription(obj,"Improves healing abilities by 25%.")
    SetItemGoldCost(obj,80)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/medicinal_herbs/medicinal_herbs.png");
end

function attached(obj)

end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)

end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
    v = value
    if (effectType == EFFECT_HEAL) then
        v = v * 1.25
    end
    return v
end

function OnDamaged(item,sourceObj,damagedObj,value)
end