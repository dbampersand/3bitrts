local timer = 0
local cd = 8

local heal = 80

local radius = 60

function setup(obj)
    SetItemName(obj,"Healing Branch")
    SetItemDescription(obj,"Occasionally creates an orb of healing at a random position nearby.")
    SetItemGoldCost(obj,25)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/healing_branch/healing_branch.png");

    
end

function attached(obj)

end

function update(dt)
    timer = timer + dt
    if (timer > cd) then
        timer = 0

        local f1 = {};
        f1["trigger"] = TRIGGER_INSTANT
        f1["type"] = EFFECT_HEAL
        f1["value"] = heal;
        
        local cast = false
        local numAttempts = 0

        while (cast == false) do
            local x = GetX(GetObjRef()) + RandRange(-radius,radius)
            local y = GetY(GetObjRef()) + RandRange(-radius,radius)
            if (IsWalkable(x,y,true) or numAttempts > 30) then
                cast = true
                local projectile = CreateProjectile(x,y,x,y,"",ATTACK_PROJECTILE_ANGLE,0,20,false,ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_HEAL,6,{f1})
                SetCanHitParent(projectile,true)
            end
            numAttempts = numAttempts + 1
        end

    end
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
