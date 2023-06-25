
function setup(obj)
    SetItemName(obj,"Barbed Wire")
    SetItemDescription(obj,"Deals damage when hit.")
    SetItemGoldCost(obj,100)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/barbed_wire/barbed_wire.png");
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function ondamaged(item,sourceObj,damagedObj,value)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 5;  

    ApplyEffect(sourceObj,{f1});
end