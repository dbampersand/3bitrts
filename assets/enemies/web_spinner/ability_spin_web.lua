local dps = 80
local ticksPerSec = 5
local duration = 40

local radius = 25

local speed = 35

function setup()

    SetAbilityRange(200)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_CIRCLE,radius);
    SetCooldown(9); 
    AbilitySetPortrait("assets/enemies/web_spinner/ability_spin_web.png");
    SetDescription("[b]Spin Web\n\nSpins a web and sends it towards a target, dealing damage and slowing.")
end

function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps / ticksPerSec;

    local f2 = {}
    f2["trigger"] = TRIGGER_CONST
    f2["type"] = EFFECT_SPEED
    f2["duration"] = 1
    f2["value"] = -40



    local aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",radius, 1 / ticksPerSec,duration,false,ATTACK_HITS_ENEMIES,COLOR_SPEED,DITHER_DAMAGE_EIGTH,false,-1,{f1,f2});
   
    local dir = {};
    dir.x = GetX(obj) - GetX(GetObjRef());
    dir.y = GetY(obj) - GetY(GetObjRef());
    SetAttackMoveAngle(aoe,dir.x,dir.y);

    PlaySound("assets/enemies/web_spinner/audio/spin_web.wav",0.5);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    SetAttackVelocity(attackRef,GetAttackVelocity(attackRef)+dt*32);
end

function parentdeath(x, y, durationLeft, parent, target, attackRef)
    RemoveAttack(attackRef);
end