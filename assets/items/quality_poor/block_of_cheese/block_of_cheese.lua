
function setup(obj)
    SetItemName(obj,"Block of Cheese")
    SetItemDescription(obj,"Regenerates health.")
    SetItemGoldCost(obj,15)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/block_of_cheese/block_of_cheese.png");

    
end

function attached(obj)
end

function update(dt,obj)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HEAL;
    f1["value"] = 5 * dt

    --ApplyEffect(obj,{f1});
    Heal(obj,5*dt);
end

function onattack(item,obj,target,dt,value)
end


function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
