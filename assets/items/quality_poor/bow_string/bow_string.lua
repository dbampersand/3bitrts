
function setup(obj)
    SetItemName(obj,"Bow String")
    SetItemDescription(obj,"Increases range.")
    SetItemGoldCost(obj,15)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/bow_string/bow_string.png");

    
end

function attached(obj)
    local f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_ATTACKRANGE
    f1["value"] = 20
    f1["name"] = "Bow String"

    ApplyEffect(obj,{f1})
end
function unattach(obj)
    CureNamedEffect(obj,"Bow String",1)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end


function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
