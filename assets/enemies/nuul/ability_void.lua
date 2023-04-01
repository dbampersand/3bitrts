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

    local randTargets = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,9999,3)
    for i=0,#randTargets do
        local xHeading = GetX(GetObjRef()) - GetX(randTargets[i+1]);
        local yHeading = GetY(GetObjRef()) - GetY(randTargets[i+1]);
       -- Print(randTargets[i+1]);
    end
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