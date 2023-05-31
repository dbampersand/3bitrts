
function setup(obj)
    SetItemName(obj,"Throwing Dagger")
    SetItemDescription(obj,"+10 Range and +2 damage")
    SetItemGoldCost(obj,60)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/throwing_dagger/throwing_dagger.png");

end

function attached(obj)
    local f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_ATTACKRANGE
    f1["value"] = 10
    f1["name"] = "Throwing Dagger"

    ApplyEffect(obj,{f1})
end
function unattach(obj)
    CureNamedEffect(obj,"Throwing Dagger",1)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    local v = value + 2
    return v;
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
