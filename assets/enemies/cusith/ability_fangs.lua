local range = 200
local radius = 20
local time = 1.25;

local attacks = {}


local timer = 0;

local numAttacks = 2

local numSegments = range / radius;
local segmentsMade = 0

local ticker = -1
local radius = 15

local startRadius = 30

local timeBeforeDamage = 0.35;



function setup()

    SetAbilityRange(500)
    --AbilitySetCastType(ABILITY_CONE);
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_CONE,radius,false,length);
    SetCooldown(4); 
    AbilitySetPortrait("assets/enemies/cusith/ability_bite.png");
    SetDescription("[b]Fangs\n\nCreates a line of spikes, hitting anything along its axis.")

end
function casted(x,y,obj,headingx,headingy)

    heading = {headingx = headingx, headingy = headingy};
    ticker = CreateTicker(time);
    
    timer = 0
    segmentsMade = 0

    startX = GetX(GetObjRef());
    startY = GetY(GetObjRef());

    local angToSpawn = -startRadius
    local move = startRadius / numAttacks
    for i = 1, numAttacks do    
        
        angToSpawn = angToSpawn + move
    end
    local posOne = RotatePoint(startX,startY,GetX(GetObjRef()),GetY(GetObjRef()),-startAngle/2.0f)

    
    attacks[1].startX = posOne.x;
    attacks[1].startY = posOne.y;

    local posOne = RotatePoint(startX,startY,GetX(GetObjRef()),GetY(GetObjRef()),-startAngle/2.0f)



    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    if (attackRef == ticker) then
        timer = timer + dt;
        if (timer > (time / numSegments) * segmentsMade) then
            
            local xMove = (segmentsMade / numSegments) * (range * heading.headingx);
            local yMove = (segmentsMade / numSegments) * (range * heading.headingy);
            
            local f1 = {};
            f1["trigger"] = TRIGGER_INSTANT;
            f1["type"] = EFFECT_HURT;
            f1["value"] = 75;
        
            local aoe = CreateAOE(startX + xMove,startY + yMove,"",radius,timeBeforeDamage,timeBeforeDamage,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,{f1});
        
            segmentsMade = segmentsMade + 1;
            PlaySound("assets/enemies/prowler/audio/spines.wav",1);

        end
    end
end

function onchanneled(obj, channelingtime, totalchanneltime, targetObj, targetX, targetY, targetHeadingX, targetHeadingY)
    SetChannelingSprite("assets/enemies/cusith/cusith_channelling_bite.png")

end