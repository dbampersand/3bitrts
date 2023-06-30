local range = 200
local radius = 20
local time = 2;

local attacks = {}
local numAttacks = 2


local timer = 0;


local numSegments = range / radius;
local segmentsMade = 0

local ticker = -1
local radius = 15

local startRadius = 80

local timeBeforeDamage = 0.35;


local damage = 40

function setup()

    SetAbilityRange(500)
    --AbilitySetCastType(ABILITY_CONE);
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_CONE,radius,false,length);
    SetCooldown(4); 
    AbilitySetPortrait("assets/enemies/cusith/ability_bite.png");
    SetAbilityName("Fangs")
    SetDescription("[b]Fangs\n\nCreates a line of fangs, hitting anything along its axis.")

end
function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/enemies/cusith/audio/fangs.wav",1)

    heading = {headingx = headingx, headingy = headingy};
    ticker = CreateTicker(time);
    
    timer = 0
    segmentsMade = 0

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;

    for i = 1, numAttacks do


        local point = {}
        point.x = (GetX(GetObjRef()) - GetX(obj))
        point.y = (GetY(GetObjRef()) - GetY(obj))
        point = Normalize(point.x,point.y);
        point.x = GetX(GetObjRef()) + (point.x * 60);
        point.y = GetY(GetObjRef()) + (point.y * 60);


        local point1 = RotatePoint(point.x,point.y, GetX(GetObjRef()), GetY(GetObjRef()), -startRadius + (startRadius / i))


        --local aoe1 = CreateAOE(point1.x,point1.y,"",radius,timeBeforeDamage,timeBeforeDamage,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,{f1});

        local dir = {};
        dir.x = GetX(obj) - point1.x;
        dir.y = GetY(obj) - point1.y;
        local nrm = Normalize(dir.x,dir.y);
        --SetAttackMoveAngle(aoe1,dir.x,dir.y);
        --SetAttackVelocity(aoe1,40)
        attacks[i] = {atk = aoe1, x = point1.x, y = point1.y, xHeading = nrm.x, yHeading = nrm.y}

    end

    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (attackRef == ticker) then
        timer = timer + dt;
            if (timer > (time / numSegments) * segmentsMade) then
                for i = 1, numAttacks do

                    local xMove = (segmentsMade / numSegments) * (range * attacks[i].xHeading);
                    local yMove = (segmentsMade / numSegments) * (range * attacks[i].yHeading);
                    
                    local f1 = {};
                    f1["trigger"] = TRIGGER_INSTANT;
                    f1["type"] = EFFECT_HURT;
                    f1["value"] = damage;
                
                    local aoe = CreateAOE(attacks[i].x + xMove,attacks[i].y + yMove,"",radius,timeBeforeDamage,timeBeforeDamage,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,{f1});
                    PlaySound("assets/enemies/prowler/audio/spines.wav",1);

                end
            segmentsMade = segmentsMade + 1;

        end
    end
end

function onchanneled(obj, channelingtime, totalchanneltime, targetObj, targetX, targetY, targetHeadingX, targetHeadingY)
    SetChannelingSprite("assets/enemies/cusith/cusith_channelling_bite.png")

end