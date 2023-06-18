local chargeTime = 0.35
local aoeSize = 32
local dps = 120
local triggerTime = 0.05

function setup()
    SetAbilityRange(128)
    SetCooldown(4);
    AbilitySetPortrait("assets/enemies/kobold_crusher/ability_charge.png");
    SetDescription("[b]Charge\n\nCharges a unit, dealing damage around them.");
    SetAbilityName("Charge"); 

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/kobold_crusher/audio/charge.wav",0.5)
    
    SetObjectPush(true);
    After(SetObjectPush,chargeTime,false,false);
    EnableAI(GetObjRef(),false)
    After(EnableAI,chargeTime,false,GetObjRef(),true)

    
    local f1 = {};  
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * triggerTime;  

    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", aoeSize, triggerTime, chargeTime, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, false, -1, {f1})


    local speed = {};
    speed["trigger"] = TRIGGER_CONST;
    speed["type"] = EFFECT_SPEED; 
    speed["value"] = 200; 
    speed["duration"] = chargeTime

    ApplyEffect(GetObjRef(),{speed});

    local heading = GetHeadingVector(GetX(GetObjRef()),GetY(GetObjRef()),x,y);
    local newX = heading.headingx * 9999;
    local newY = heading.headingy * 9999;
    SetMovePoint(GetObjRef(),newX,newY,false,false);
    
    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    MoveAttack(attackRef,GetX(parent),GetY(parent));
end
function onchanneled() 
end