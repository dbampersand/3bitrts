local increase = 0.65
local radius = 25
local applied = false
function setup(obj)
    SetItemName(obj,"Lonely Star")
    SetItemDescription(obj,"If the unit is " .. radius .. " units away from another friendly unit, increase ability potency by ".. increase * 100 .. "%")
    SetItemGoldCost(obj,40)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/lonely_star/lonely_star.png");
end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt,item)
    local shouldHighlight = true
    for i = 0, MAX_OBJS-1 do

        if (i ~= GetObjRef() and IsAlive(i) and GetObjFriendliness(GetObjRef()) == GetObjFriendliness(i)) then
            if (GetDist(GetObjRef(),i) < radius) then
                shouldHighlight = false
            end
        end
    end
    if (shouldHighlight) then
        SetItemHighlight(item,true)
    else
        SetItemHighlight(item,false)
    end
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
function onabilitycast(item,obj,target,ability,x,y)
    if (applied) then
        applied = false
        AddAbilityPotency(GetObjRef(),-increase)
    end
end

function beforeabilitycast(item,obj,target,ability,x,y) 
    local applyBonus = true
    for i = 0, MAX_OBJS-1 do

        if (i ~= GetObjRef() and IsAlive(i) and GetObjFriendliness(GetObjRef()) == GetObjFriendliness(i)) then
            if (GetDist(GetObjRef(),i) < radius) then
                applyBonus = false
            end
        end
    end
    if (applyBonus) then
        AddAbilityPotency(GetObjRef(),increase)
        applied = true
    end
end