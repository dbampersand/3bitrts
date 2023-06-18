local dps = 10
local duration = 60
local ticksPerSec = 3
local radius = 60

local casts = 0

local changedDirection = false
local direction = 0
local velocity = 0

local speedPerSec = 5
local speedUp = 16


function setup()

    SetAbilityRange(200)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_CIRCLE,radius);
    SetCooldown(15); 
    AbilitySetPortrait("assets/enemies/giant_worm/ability_wave.png");
    SetDescription("[b]Wave\n\nCalls a curing wave affecting all targets.")
end

function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_CURE;
    f1["value"] = 1;


    if (math.fmod(casts,2) == 0) then
        for i = 0,GetMapHeight()/radius+1 do
            local atk = CreateAOE(1,i*radius,"",radius,0.25,9999,false,Bor(ATTACK_HITS_FRIENDLIES, ATTACK_HITS_ENEMIES),COLOR_HEAL,DITHER_HEAL_HALF,false,-1,{f1});
            SetAttackMoveAngle(atk,1,0)
            SetAttackVelocity(atk,2)  
        end
        direction = 1
    else
        for i = 0,GetMapHeight()/radius+1 do
            local atk = CreateAOE(GetMapWidth()-1,i*radius,"",radius,0.25,9999,false,Bor(ATTACK_HITS_FRIENDLIES, ATTACK_HITS_ENEMIES),COLOR_HEAL,DITHER_HEAL_HALF,false,-1,{f1});
            SetAttackMoveAngle(atk,1,0)
            SetAttackVelocity(atk,-2)
        end
        direction = -1
    end
    casts = casts + 1
    changedDirection = false

    PlaySound("assets/enemies/giant_worm/audio/wave.wav",0.75);
    return true; 
end

function onhit(x,y,objhit)
    SetChannelingSprite("assets/enemies/giant_worm/giant_worm_casting_wave.png");
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    

    if (direction == 1 and x > GetMapWidth()/2) then
        changedDirection = true
    end
    if (direction == -1 and x < GetMapWidth()/2) then
        changedDirection = true
    end

    if (changedDirection) then
        SetAttackVelocity(attackRef,GetAttackVelocity(attackRef)+(dt*speedPerSec*speedUp*-direction))
    else
        SetAttackVelocity(attackRef,GetAttackVelocity(attackRef)+(dt*speedPerSec*direction))
    end

end

function parentdeath(x, y, durationLeft, parent, target, attackRef)
end