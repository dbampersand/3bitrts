local percentHP = 30

function setup(obj) 
    SetItemName(obj,"Reaper's Breath")
    SetItemDescription(obj,"When lower than " .. percentHP .. "% hp, double your attack damage.")
    SetItemGoldCost(obj,70)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/reapers_breath/reapers_breath.png");
end

function onmapchange(obj)
end

function attached(obj)
end

function update(dt,item)

    if (GetHPPercent(GetObjRef()) < percentHP) then
        SetItemHighlight(item,true)
    else
        SetItemHighlight(item,false)
    end


end


function onattack(item,obj,target,dt,value)
    if (GetHPPercent(GetObjRef()) < percentHP) then
        value = value * 2
    end
    return value
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
function onabilitycast(item,obj,target,ability,x,y)
end

function beforeabilitycast(item,obj,target,ability,x,y) 
end