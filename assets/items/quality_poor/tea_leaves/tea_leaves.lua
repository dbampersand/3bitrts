local AS = 0.1
local damage = 8

function setup(obj)
    SetItemName(obj,"Tea Leaves")
    SetItemDescription(obj,"Slows your attack speed, but makes each hit deal more damage.")
    SetItemGoldCost(obj,15)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/bow_string/bow_string.png");

    
end

function attached(obj)
    local f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_ATTACKSPEED
    f1["value"] = AS
    f1["name"] = "Tea Leaves"

    local f2 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_ATTACKDAMAGE    
    f1["value"] = damage
    f1["name"] = "Tea Leaves"


    ApplyEffect(obj,{f1})
end
function unattach(obj)
    CureNamedEffect(obj,"Tea Leaves",1)
    CureNamedEffect(obj,"Tea Leaves",1)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end


function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
