local duration = 6
local makeAttackAfter = 0.35

local timer = 0

local dps = 20
local slow = -20
local ticksPerSecond = 4

local ticker = 0

local radius = 25

function setup()

    SetAbilityRange(999)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE);
    SetCooldown(30); 
    AbilitySetPortrait("assets/enemies/naja/ability_sinkholes.png");
    SetDescription("[b]Sinkholes\n\nRandomly creates dozens of damaging and slowing areas.")
end
function casted(x,y,obj,headingx,headingy)

    ticker = CreateTicker(duration);

    timer = makeAttackAfter;
    
    
    PlaySound("assets/enemies/naja/audio/bind_cast.wav",1);

    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (attackRef == ticker) then
        timer = timer - dt
        if (timer < 0) then
            timer = makeAttackAfter;

            local f1 = {};
            f1["trigger"] = TRIGGER_INSTANT;
            f1["type"] = EFFECT_HURT;
            f1["value"] = dps / ticksPerSecond;

            local f2 = {}
            f2["trigger"] = TRIGGER_CONST;
            f2["type"] = EFFECT_SPEED;
            f2["value"] = slow;
            f2["duration"] = 1
            f2["overwrites"] = true;
            f2["name"] = "Sinkhole"


            local randX = RandRange(0,GetMapWidth());
            local randY = RandRange(0,GetMapHeight());

            local aoe = CreateAOE(randX,randY,"",radius,1/ticksPerSecond,9,false,ATTACK_HITS_ENEMIES,COLOR_SPEED,DITHER_DAMAGE_HALF,false,-1,{f1,f2});
        end
    end
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/naja/naja_cast_sinkholes.png")
end