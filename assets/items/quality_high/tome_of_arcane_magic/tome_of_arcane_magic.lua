local total = 1.75

function setup(obj)
    SetItemName(obj,"Tome of Arcane Magic")
    SetItemDescription(obj,"Adds extra ability potency proportional to how much health percent is missing.")
    SetItemGoldCost(obj,140)
    SetItemTier(obj,2)
    SetItemIcon(obj, "assets/items/quality_high/tome_of_arcane_magic/tome_of_arcane_magic.png");

end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt)
end
function beforeabilitycast(item,obj,target,ability,x,y) 
    local add = (1 - (GetHPPercent(GetObjRef()) / 100)) * total
    AddAbilityPotency(GetObjRef(),add)
end
function onabilitycast(item,obj,target,ability,x,y)
    local add = (1 - (GetHPPercent(GetObjRef()) / 100)) * total
    
    AddAbilityPotency(GetObjRef(),-add)
end

