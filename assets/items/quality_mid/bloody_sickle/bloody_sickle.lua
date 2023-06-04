local percent = 2.25
local chance = 0.2
function setup(obj)
    SetItemName(obj,"Bloody Sickle")
    SetItemDescription(obj,"When attacking, adds a " .. chance * 100 .. "chance to lifesteal " .. percent*100 .. "%")
    SetItemGoldCost(obj,110)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/bloody_sickle/bloody_sickle.png");

end

function attached(obj)

end

function unattach(obj)
end
function onmapchange(obj)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    local roll = RandRange(0,1)
    if (chance > roll) then
        Heal(obj,value*percent)
    end
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function onabilitycast(item,obj,target,ability,x,y)
end
