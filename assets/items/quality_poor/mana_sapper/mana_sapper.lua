local manaToAdd = 2

function setup(obj)
    SetItemName(obj,"Mana Stone")
    SetItemDescription(obj,"Attacking adds " .. manaToAdd .. " to your mana pool.")
    SetItemGoldCost(obj,35)
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
    AddMana(obj,manaToAdd)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

