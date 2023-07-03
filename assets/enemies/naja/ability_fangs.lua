local radius = 20
local angleFrom = 30

local dps = 140
local ticksPerSecond = 5

function setup()

    SetAbilityRange(999)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE);
    SetCooldown(8); 
    AbilitySetPortrait("assets/enemies/naja/ability_fangs.png");
    SetDescription("[b]Fangs\n\nCreates a pair of damaging areas which fly at the target.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps / ticksPerSecond;


    --start point, at the opposite side of the object to the target
    local point = {}
    point.x = (GetX(GetObjRef()) - GetX(obj))
    point.y = (GetY(GetObjRef()) - GetY(obj))
    point = Normalize(point.x,point.y);
    point.x = GetX(GetObjRef()) + (point.x * 60);
    point.y = GetY(GetObjRef()) + (point.y * 60);


    local point1 = RotatePoint(point.x,point.y, GetX(GetObjRef()), GetY(GetObjRef()), -angleFrom)
    local point2 = RotatePoint(point.x,point.y, GetX(GetObjRef()), GetY(GetObjRef()), angleFrom)


    local aoe1 = CreateAOE(point1.x,point1.y,"",radius,1/ticksPerSecond,9,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,0,0,0,{f1});
    local aoe2 = CreateAOE(point2.x,point2.y,"",radius,1/ticksPerSecond,9,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,0,0,0,{f1});

    local dir = {};
    dir.x = GetX(obj) - point1.x;
    dir.y = GetY(obj) - point1.y;
    SetAttackMoveAngle(aoe1,dir.x,dir.y);
    SetAttackVelocity(aoe1,40)

    dir.x = GetX(obj) - point2.x;
    dir.y = GetY(obj) - point2.y;
    SetAttackMoveAngle(aoe2,dir.x,dir.y);
    SetAttackVelocity(aoe2,40)


    PlaySound("assets/enemies/naja/audio/fangs.wav",0.75);
    After(PlaySound,0.1,true,"assets/enemies/naja/audio/fangs.wav",0.75);

    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    SetAttackVelocity(attackRef,GetAttackVelocity(attackRef)+dt*32);
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/naja/naja_cast_fangs.png")
end