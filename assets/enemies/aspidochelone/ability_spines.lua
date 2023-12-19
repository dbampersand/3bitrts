local damage = 10

local radius = 30

function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(12); 
    AbilitySetPortrait("assets/enemies/aspidochelone/ability_spines.png");
    SetDescription("[b]Spines\n\nSeveral spines are shot towards targets.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = damage 

    local enemies = GetAllObjsByFrie    ndliness(GetOppositeFriendliness(GetObjRef()));
    for i = 1,#enemies do
        CreateConeProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),GetX(enemies[i]),GetY(enemies[i]),"",ATTACK_PROJECTILE_ANGLE,80,15,true,ATTACK_HITS_ENEMIES,6,COLOR_DAMAGE,radius,{f1});

    end
    PlaySound("assets/enemies/aspidochelone/audio/ability_spines.wav",1.0,true)

    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,parent,dt,target,attack)
end
function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
end

function applyattack(atk,x,y)
end
function timerbeforetick(t,func)
end