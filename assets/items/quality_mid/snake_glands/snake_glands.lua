local cooldown = 12
local timer = 0

function setup(obj)
    SetItemName(obj,"Snake Glands")
    SetItemDescription(obj,"Cures a single bad effect every ".. cooldown .. " seconds")
    SetItemGoldCost(obj,55)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/snake_glands/snake_glands.png");

end

function attached(obj)

end

function unattach(obj)
end

function update(dt)
    timer = timer + dt
    if (timer > cooldown) then
        timer = 0

        local f1 = {};
        f1["trigger"] = TRIGGER_INSTANT
        f1["type"] = EFFECT_CURE
        f1["value"] = 1;
        
        ApplyEffect(obj,{f1})
    end

end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
