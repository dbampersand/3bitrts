local maxStacks = 2
local lastPosition = {x = 0, y = 0}

local amountAdded = 0

local moveMultiplier = 0.008

function setup(obj)
    SetItemName(obj,"Silver Boots")
    SetItemDescription(obj,"Moving increases your ability potency to a max of " .. maxStacks*100 .. "%")
    SetItemGoldCost(obj,110)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/silver_boots/silver_boots.png");

end

function attached(obj)

end

function unattach(obj)
end
function onmapchange(obj)
    lastPosition.x = GetX(obj)
    lastPosition.y = GetY(obj)

end

function update(dt,item)
    local currX = GetX(GetObjRef())
    local currY = GetY(GetObjRef())

    local amt = DistXY(currX,currY,lastPosition.x,lastPosition.y) * moveMultiplier;

    if (amountAdded < maxStacks) then
        SetItemHighlight(item,false)
        
        AddAbilityPotency(GetObjRef(),amt);

        amountAdded = amountAdded + amt
    else
        SetItemHighlight(item,true)

    end

    lastPosition.x = currX;
    lastPosition.y = currY;

    SetItemStackString(item,string.format("%.1f", amountAdded))
    
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function onabilitycast(item,obj,target,ability,x,y)
    AddAbilityPotency(GetObjRef(),-amountAdded);
    amountAdded = 0
end
