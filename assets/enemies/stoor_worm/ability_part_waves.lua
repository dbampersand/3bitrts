
local numAoEs = 12

local partPointStart = 5
local partPointEnd = 8


local cooldown = 8
local tickrate = 0.5
local dps = 25
local radius = 40

local accel = 8
function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_summon_adds.png");
    SetAbilityName("Part Waves");
    SetDescription("[b]Part Waves\n\nParts the waves, creating an attack that emanates from the centre.");
    SetCooldown(cooldown)

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/wyrm_boss/audio/summon_add.wav",1)
    radius = GetMapHeight()/numAoEs;

    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate

    for i = 1, numAoEs do
        local startSpeed = 0
        if (i >= partPointStart and i <= partPointEnd) then
            startSpeed = 20
        end
        CreateAOE(GetMapWidth()/2,radius*i,"",radius,tickrate,9999,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_VERTICAL_HALF,false,-1,-1,0,startSpeed,{f1});
        CreateAOE(GetMapWidth()/2,radius*i,"",radius,tickrate,9999,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_VERTICAL_HALF,false,-1,1,0,startSpeed,{f1});

    end
    

    numCasts = numCasts + 1
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
        SetAttackVelocity(attackRef,GetAttackVelocity(attackRef)+dt*accel)

end