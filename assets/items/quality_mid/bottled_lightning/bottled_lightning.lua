local damage = 30
local speed = 35

function setup(obj)
    SetItemName(obj,"Bottled Lightning")
    SetItemDescription(obj,"When attacking, create a damaging projectile which can hit enemies or allies.")
    SetItemGoldCost(obj,80)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/bottled_lightning/bottled_lightning.png");

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
    f1["type"] = EFFECT_HURT
    f1["value"] = damage;

    projectile = CreateProjectile(GetX(obj),GetY(obj),GetX(target),GetY(target),"",ATTACK_PROJECTILE_ANGLE,speed,999,false,Bor(ATTACK_HITS_ENEMIES,ATTACK_HITS_FRIENDLIES),COLOR_FRIENDLY_DAMAGE,1,{f1})

end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
