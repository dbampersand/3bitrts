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
    SetCooldown(14); 
    AbilitySetPortrait("assets/enemies/prowler/ability_snare.png");
    SetDescription("[b]Snare\n\nEntraps the target.")
end
function onchanneled()
end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/prowler/audio/snare.wav",0.5)
    local f1 = {};
    f1["trigger"] = TRIGGER_CONST; 
    f1["type"] = EFFECT_SPEED;
    f1["value"] = -40;
    f1["duration"] = 1.5;
    f1["overwrites"] = true;
    f1["name"] = "Snare";

    CreateAOE(x,y,"", 60, 0.1, 12, false, ATTACK_HITS_ENEMIES, COLOR_SPEED, DITHER_HORIZONTAL_QUARTER, false, obj, 0,0,0,{f1})
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (attackRef == ticker) then
        timer = timer + dt;
        if (timer > (time / numSegments) * segmentsMade) then
            
            local xMove = (segmentsMade / numSegments) * (range * heading.headingx);
            local yMove = (segmentsMade / numSegments) * (range * heading.headingy);
            
            local f1 = {};
            f1["trigger"] = TRIGGER_INSTANT;
            f1["type"] = EFFECT_HURT;
            f1["value"] = 75;
        
            local aoe = CreateAOE(startX + xMove,startY + yMove,"",radius,timeBeforeDamage,timeBeforeDamage,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,0,0,0,{f1});
        
            segmentsMade = segmentsMade + 1;
            PlaySound("assets/enemies/prowler/audio/spines.wav",1);

        end
    end
end