
function setup(obj)
    SetItemName(obj,"Wand of Frost")
    SetItemDescription(obj,"Slows enemies and their attack ")
    SetItemGoldCost(obj,25)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/sharpening_stone/sharpening_stone.png");

    
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    local v = value + 2
    return v;
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
