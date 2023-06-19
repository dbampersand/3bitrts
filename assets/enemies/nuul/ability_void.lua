local attacks = {};
local numAttacks = 3

local dps = 15
local tickrate = 0.2
function setup()

    SetAbilityRange(range)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_CIRCLE,30);
    SetCooldown(16); 
    AbilitySetPortrait("assets/enemies/nuul/ability_void.png");
    SetDescription("[b]Void\n\nCreates a line of void magic, hitting anything along its axis.")
end
function onchanneled()
end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/nuul/audio/void.wav",1)


    local randTargets = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,9999,3)
    for i=0,#randTargets-1 do
        local atk = {};
        
        local xHeading = GetX(randTargets[i+1]) - GetX(GetObjRef()) 
        local yHeading = GetY(randTargets[i+1]) - GetY(GetObjRef())  
        
        local f1 = {};
        f1["trigger"] = TRIGGER_INSTANT;
        f1["type"] = EFFECT_HURT;
        f1["value"] = dps * tickrate;

        local size = 40 - i * 10
        
        atk = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", size, tickrate, 6, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_EIGTH, false, -1, {f1})
        SetAttackMoveAngle(atk,xHeading,yHeading);
        SetAttackVelocity(atk,(i+1)*10);
    end
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
end
function applyattack(a,x,y)
    PlaySound("assets/enemies/nuul/audio/void_tick.wav",0.25,x,y)
end