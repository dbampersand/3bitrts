local percent = 50
local mps = 0.75
function setup(obj)
    SetItemName(obj,"Mana Battery")
    SetItemDescription(obj,"When lower than " .. percent .. "% health, increases mana regen rate.")
    SetItemGoldCost(obj,15)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/mana_battery/mana_battery.png");

    
end

function onmapchange(obj)
end

function attached(obj)

end

function update(dt)
    if (GetHPPercent(GetObjRef()) < percent) then
        AddMana(GetObjRef(),mps*dt);
    end
end

function onattack(item,obj,target,dt,value)
    
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
