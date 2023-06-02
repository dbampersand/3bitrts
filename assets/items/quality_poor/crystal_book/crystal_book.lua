local hp = 35

function setup(obj)
    SetItemName(obj,"Mana Stone")
    SetItemDescription(obj,"Ability casts heal you for " .. hp .. " health.")
    SetItemGoldCost(obj,40)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/mana_stone/mana_stone.png");
end

function attached(obj)
end
function unattach(obj)
end


function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
function onabilitycast(item,obj,target,ability,x,y)
    Heal(obj,hp)
end