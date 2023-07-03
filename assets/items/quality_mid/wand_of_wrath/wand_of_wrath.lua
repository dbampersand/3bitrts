local aoe = -1

local tickRate = 0.5
local dps = 120

local attackChance = 0.05

local dir = {}

local speedGain = 10

function setup(obj)
    SetItemName(obj,"Wand of Wrath")
    SetItemDescription(obj,"When attacking, sometimes launch an orb of damage.")
    SetItemGoldCost(obj,100)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/earth_shaking_boots/earth_shaking_boots.png");

end

function onmapchange(obj)
    aoe = -1
end

function attached(obj)

end

function update(dt)
    if (aoe >= 0) then
        if (AttackIsActive(aoe) == false) then
            aoe = -1
        end
        SetAttackVelocity(aoe,GetAttackVelocity(aoe)+(dt*speedGain))
    end
end

function onattack(item,obj,target,dt,value)

    if (attackChance > RandRange(0,1)) then
        local f1 = {}
        f1["trigger"] = TRIGGER_INSTANT;
        f1["type"] = EFFECT_HURT;
        f1["value"] = dps * tickRate;  

        dir = GetHeadingVector(GetX(obj),GetY(obj),GetX(target),GetY(target))
        aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, tickRate, 999, false, ATTACK_HITS_ENEMIES, COLOR_FRIENDLY_DAMAGE, DITHER_DAMAGE_HALF, false, obj,0,0,0, {f1})

        SetAttackMoveAngle(aoe,dir.headingx,dir.headingy)
        SetAttackVelocity(aoe,0)

    end

end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
