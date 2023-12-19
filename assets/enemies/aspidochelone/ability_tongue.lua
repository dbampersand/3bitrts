local damage = 100

local radius = 80

function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(12); 
    AbilitySetPortrait("assets/enemies/aspidochelone/ability_tongue.png");
    SetDescription("[b]Tongue\n\nPulls all enemies towards the user.")
end
function casted(x,y,obj,headingx,headingy)
    local enemies = GetAllObjsByFriendliness(GetOppositeFriendliness(GetObjRef()));
    for i = 1,#enemies do
        PushObj(GetX(GetObjRef()),GetY(GetObjRef()),enemies[i],-600,0.1)
    end
    PlaySound("assets/enemies/aspidochelone/audio/ability_tongue.wav",1.0,true)

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