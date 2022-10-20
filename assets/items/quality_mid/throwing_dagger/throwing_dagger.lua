
function setup(obj)
    SetItemName(obj,"Throwing Dagger")
    SetItemDescription(obj,"+2 Range and +1 damage")
    SetItemGoldCost(obj,8)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/throwing_dagger/throwing_dagger.png");

end

function Attached(item,obj)
    f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_ATTACKRANGE
    f1["value"] = 2
    f1["name"] = "Throwing Dagger"

    ApplyEffect(obj,f1)
end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)
    local v = value + 1
    return v;
end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
