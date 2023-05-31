local timer = 0

local maxManaAdd = 60
local manaRegenAdd = 0.5

function setup(obj)
    SetItemName(obj,"Mana Stone")
    SetItemDescription(obj,"Increases your total mana and mana regeneration rate.")
    SetItemGoldCost(obj,35)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/mana_stone/mana_stone.png");
end

function attached(obj)

    local f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_MAXMANA
    f1["value"] = maxManaAdd
    f1["name"] = "Mana Stone"


    local f2 = {};
    f2["trigger"] = TRIGGER_PERMANENT;
    f2["type"] = EFFECT_MANAREGEN
    f2["value"] = manaRegenAdd
    f2["name"] = "Mana Stone"


    ApplyEffect(obj,{f1,f2})


end
function unattach(obj)
    CureNamedEffect(obj,"Mana Stone",1)
    CureNamedEffect(obj,"Mana Stone",1)
end


function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
