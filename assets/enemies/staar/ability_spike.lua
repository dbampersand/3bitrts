local tickrate = 0.5
local duration = 4
local dps = 10

local damage = 20

local radius = 15
local lengthMax = 80
local numCones = 6

local cones = {}

local minRange = 30;

local atks = {}

function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(2); 
    AbilitySetPortrait("assets/enemies/staar/ability_whip.png");
    SetDescription("[b]Spike\n\nLaunches exploding projectiles at ranged units.")
end
function casted(x,y,obj,headingx,headingy)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;


    local objs = GetAllObjsByFriendliness(GetOppositeFriendliness(GetObjRef()));
    atks = {}
    for i = 1, #objs do
       if (GetDist(GetObjRef(),objs[i]) > minRange) then
            atks[i] = {}
            atks[i].atk  = CreateProjectile(GetX(GetObjRef()),GetY(GetObjRef()),GetX(objs[i]),GetY(objs[i]),"",ATTACK_PROJECTILE_ANGLE,25,16,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,1,{f1})
            atks[i].timer = RandRange(0.4,0.5);
       end
    end
    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,parent,dt,target,attack)
end
function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
    for i = 1, #atks do 
        if (attackRef == atks[i].atk) then
            atks[i].timer = atks[i].timer - dt
            if (atks[i].timer < 0) then
                local f1 = {}
                f1["trigger"] = TRIGGER_INSTANT;
                f1["type"] = EFFECT_HURT;
                f1["value"] = dps;
            
                CreateAOE(x,y,"",radius,0.1,0.1,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_NONE,false,-1,0,0,0,{f1});
                atks[i].timer = tickrate
                Print("x: " .. x .. " y: " .. y)
            end
        end
    end
end

function applyattack(atk,x,y)
end
function timerbeforetick(t,func)
end