local tickrate = 0.5
local duration = 12
local dps = 30

local radius = 35

local numCones = 8

local changeDirectionTime = 1

function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(12); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Sea Shell\n\nCreates several damaging cones.")
end
function casted(x,y,obj,headingx,headingy)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate;

    local atk = {}


    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,parent,dt,target,attack)
    local index = -1
    for i = 1,#aoes do
        if (aoes[i].aoe == attack) then
            index = i
        end
    end
    if (index >= 0) then
        table.remove(aoes,index)
    end
end
function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)

    local index = -1
    for i = 1,#aoes do
        if (aoes[i].aoe == attackRef) then
            index = i
        end
    end
    if (index == -1) then
        do return end;
    end

    aoes[index].timer = aoes[index].timer + dt

    if (aoes[index].timer >= changeDirectionTime) then
        aoes[index].towards.x = RandRange(-1,1)
        aoes[index].towards.y = RandRange(-1,1)
        aoes[index].timer = 0
    end
    
    local xTarg = aoes[index].towards.x;
    local yTarg = aoes[index].towards.y;

    local targNew = GetAttackMoveAngle(attackRef)
    targNew.x = Lerp(targNew.x,xTarg,dt)
    targNew.y = Lerp(targNew.y,yTarg,dt)

    SetAttackMoveAngle(attackRef,targNew.x,targNew.y)

end

function applyattack(atk,x,y)
end
function timerbeforetick(t,func)
end