local time = 1
local chance = 0.05

function setup(obj)
    SetItemName(obj,"Heavy Gauntlet")
    SetItemDescription(obj,"When attacking, sometimes stuns the target for ".. time .." seconds.")
    SetItemGoldCost(obj,50)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/heavy_gauntlet/heavy_gauntlet.png");

end

function attached(obj)

end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)
    if (chance >= RandRange(0,1)) then
        Stun(obj,time);
    end
end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
