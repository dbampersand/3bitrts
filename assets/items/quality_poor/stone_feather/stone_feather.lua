local AS = 0.05

function setup(obj)
    SetItemName(obj,"Stone Feather")
    SetItemDescription(obj,"Increases your attack speed by ".. AS*100 .. "%")
    SetItemGoldCost(obj,30)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/stone_feather/stone_feather.png");
end

function attached(obj)
    f1 = {};
    f1["trigger"] = TRIGGER_PERMANENT;
    f1["type"] = EFFECT_ATTACKSPEED
    f1["value"] = -AS
    f1["name"] = "Stone Feather"

    ApplyEffect(obj,{f1})
end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)
end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
