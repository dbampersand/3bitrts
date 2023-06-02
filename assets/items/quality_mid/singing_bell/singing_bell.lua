local heal = 30
local speed = 60

function setup(obj)
    SetItemName(obj,"Singing Bell")
    SetItemDescription(obj,"When attacking, create a healing projectile.")
    SetItemGoldCost(obj,90)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/singing_bell/singing_bell.png");

end

function attached(obj)

end

function unattach(obj)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HEAL
    f1["value"] = heal;

    projectile = CreateProjectile(GetX(obj),GetY(obj),GetX(target),GetY(target),"",ATTACK_PROJECTILE_ANGLE,speed,999,false,ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_HEAL,1,{f1})

end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
