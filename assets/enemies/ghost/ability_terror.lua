local duration = 12
local dps = 40
local tickRate = 0.5

local radius = 25


local angle = RandRange(0,360)
local angleMove = 30

function setup()
    SetAbilityRange(200)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_CIRCLE,radius);
    SetCooldown(2); 
    SetAbilityMaxStacks(11,11)
    AbilitySetPortrait("assets/enemies/ghost/ability_terror.png");
    SetDescription("[b]Terror\n\nCreates a moving damaging area.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickRate;

    local moveAngle = DegToHeadingVector(angle);
    local atk = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius, tickRate, duration, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_EIGTH, false, -1, {f1,f2})
    
    SetAttackMoveAngle(atk,moveAngle.headingx,moveAngle.headingy)
    SetAttackVelocity(atk,30)
    SetAttackInactive(atk,0.25)

    PlaySound("assets/enemies/viper/audio/bite.wav",1);

    angle = angle + angleMove

    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end