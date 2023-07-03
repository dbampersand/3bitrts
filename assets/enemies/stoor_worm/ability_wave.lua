
local numAoEs = 20
local numCasts = 0

local tickrate = 0.5
local dps = 40
local accel = 14

local wave1Sign = 1
local wave2Sign = 1

local cooldown = 18

function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_summon_adds.png");
    SetAbilityName("Wave");
    SetDescription("[b]Wave\n\nCreates areas of highly-damaging waves.");
    SetCooldown(cooldown)

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/wyrm_boss/audio/summon_add.wav",1)
    local aoeRadius = GetMapWidth()/3
    local numAoEs = GetMapHeight()/aoeRadius

    local x =  GetMapWidth()/3

    if (math.fmod(numCasts,2) == 0) then
        x = GetMapWidth() - (GetMapWidth()/3)
    end
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;

    for i = 1, numAoEs do
        After(CreateAOE,1+i,true,x,0,"",aoeRadius,tickrate,cooldown,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_EIGTH,false,-1,0,1,20,{f1})
        After(CreateAOE,3+i,true,GetMapWidth()/2,0,"",aoeRadius,tickrate,cooldown,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_EIGTH,false,-1,0,1,20,{f1})
    end

    wave1Sign = 1
    wave2Sign = 1

    

    numCasts = numCasts + 1
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
    SetAttackMoveAngle(attackRef,0,1)

    local sign = 1

    if (y > GetMapHeight()-1) then
        if (x == GetMapWidth()/2) then
            wave2Sign = -1
        else   
            wave1Sign = -1
        end
    end
    if (x == GetMapWidth()/2) then
        SetAttackVelocity(attackRef,GetAttackVelocity(attackRef)+dt*accel*wave2Sign)
    else
        SetAttackVelocity(attackRef,GetAttackVelocity(attackRef)+dt*accel*wave1Sign)
    end

end