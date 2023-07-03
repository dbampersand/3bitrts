local damage = 30

function setup(obj)
    SetItemName(obj,"Pyromancer's Staff")
    SetItemDescription(obj,"When attacking, deals " .. damage .." damage in an area.")
    SetItemGoldCost(obj,180)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/pyromancers_staff/pyromancers_staff.png");
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;  

    CreateAOE(GetX(target),GetY(target),"", 30, 0.1,0.1, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, -1,0,0,0, {f1})

end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function ondamaged(item,sourceObj,damagedObj,value)


end