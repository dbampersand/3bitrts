function setup(obj)
    SetItemName(obj,"Lifestone")
    SetItemDescription(obj,"When killed, ressurect the unit with full health and destroys this item.")
    SetItemGoldCost(obj,150)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/lifestone/lifestone.png");
end

function attached(obj)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function ondamaged(item,sourceObj,damagedObj,value)
    if (GetHP(damagedObj) - value < 0) then
        Heal(damagedObj,GetMaxHP(damagedObj))
        RemoveItem(damagedObj,item);
        return 0;
    end
end
function onmapchange(obj)
end