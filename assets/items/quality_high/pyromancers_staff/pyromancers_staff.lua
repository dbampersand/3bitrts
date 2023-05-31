
function setup(obj)
    SetItemName(obj,"Pyromancer's Staff")
    SetItemDescription(obj,"Deals damage when hit.")
    SetItemGoldCost(obj,160)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/barbed_wire/barbed_wire.png");
end

function attached(obj)

end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)
end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function OnDamaged(item,sourceObj,damagedObj,value)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 10;  

    ApplyEffect(sourceObj,{f1});
end