local attacks = {};
local numAttacks = 3
function setup()

    SetAbilityRange(range)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_CIRCLE,30);
    SetCooldown(3); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Void\n\nCreates a line of void magic, hitting anything along its axis.")
end
function onchanneled()
end

function casted(x,y,obj,headingx,headingy)

    local attack = {};

    --local randTargets = GetRandomUnit

    Print("headingx: " .. headingx .. ", headingy: " .. headingy);
    local xHeading = GetX(GetObjRef() - )
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
            
            local xMove = (segmentsMade / numSegments) * (range * heading.headingx);
            local yMove = (segmentsMade / numSegments) * (range * heading.headingy);
            
            local f1 = {};
            f1["trigger"] = TRIGGER_INSTANT;
            f1["type"] = EFFECT_HURT;
            f1["value"] = 35;
        
            local aoe = CreateAOE(startX + xMove,startY + yMove,"",radius,timeBeforeDamage,timeBeforeDamage,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,{f1});
        
            segmentsMade = segmentsMade + 1;
            PlaySound("assets/enemies/prowler/audio/spines.wav",1);

        end
    end
end