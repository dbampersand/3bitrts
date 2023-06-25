local damage = 15
local chance = 0.3

local cooldown = 0.5
local cdTimer = 0
function setup(obj)
    SetItemName(obj,"Spiked Armour")
    SetItemDescription(obj,"When taking damage, sometimes causes a range of projectiles to emit.")
    SetItemGoldCost(obj,105)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/spiked_armour/spiked_armour.png");

end

function attached(obj)

end

function unattach(obj)
end

function update(dt)
    cdTimer = cdTimer - dt
    if (cdTimer < 0) then
        cdTimer = 0
    end
end
function ondamaged(item,sourceObj,damagedObj,value)
    if (cdTimer > 0) then
        do return end
    end
    local roll = RandRange(0,1)


    if (roll < chance) then
        cdTimer = cooldown
        local f1 = {};
        f1["trigger"] = TRIGGER_INSTANT;
        f1["type"] = EFFECT_HURT
        f1["value"] = damage;  

        CreateCircularProjectiles(GetX(damagedObj),GetY(damagedObj),"",ATTACK_PROJECTILE_ANGLE,80,2,false,ATTACK_HITS_ENEMIES,12,COLOR_DAMAGE,0,{f1})
    end
end
function onattack(item,obj,target,dt,value)

end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)

end
