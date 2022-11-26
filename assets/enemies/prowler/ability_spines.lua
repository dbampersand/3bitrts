local range = 200
local radius = 20
local time = 1.25;

local heading = {};
local timer = 0;

local numSegments = range / radius;
local segmentsMade = 0

local ticker = -1
local radius = 15

local startX = 0;
local startY = 0;

local timeBeforeDamage = 0.35;

function setup()

    SetAbilityRange(range)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_CIRCLE,30);
    SetCooldown(3); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Acid\n\nQuickly deals damage.")
end
function onchanneled()
    SetChannelingSprite("assets/enemies/giant_ooze/giant_ooze_channelling.png");
end

function casted(x,y,obj,headingx,headingy)

    Print("headingx: " .. headingx .. ", headingy: " .. headingy);

    heading = {headingx = headingx, headingy = headingy};
    ticker = CreateTicker(time);
    
    timer = 0
    segmentsMade = 0

    startX = GetX(GetObjRef());
    startY = GetY(GetObjRef());
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (attackRef == ticker) then
        timer = timer + dt;
        if (timer > (time / numSegments) * segmentsMade) then
            Print("time: " .. time .. ", numSegments: " .. numSegments .. "segmentsMade: " .. segmentsMade)
            
            local xMove = (segmentsMade / numSegments) * (range * heading.headingx);
            local yMove = (segmentsMade / numSegments) * (range * heading.headingy);
            
            local f1 = {};
            f1["trigger"] = TRIGGER_INSTANT;
            f1["type"] = EFFECT_HURT;
            f1["value"] = 100;
        
            local aoe = CreateAOE(startX + xMove,startY + yMove,"",radius,timeBeforeDamage,timeBeforeDamage,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,{f1});
        
            segmentsMade = segmentsMade + 1;

        end
    end
end