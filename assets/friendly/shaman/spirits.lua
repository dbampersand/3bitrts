local seconds = 2

local damage = 120

local radius = 25
function setup()
    AbilitySetPortrait("assets/friendly/shaman/icon_spirits.png");
    SetDescription("Spirits\n\nSummons a deadly spirit, exploding after " ..seconds.." damage to **all** units.")

    AbilitySetCastType(ABILITY_POINT);
    SetAbilityHint(HINT_CIRCLE,radius)
    SetAbilityRange(80)

    SetCooldown(2);
end


function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/friendly/shaman/audio/spirit.wav",1);


    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;   
    f1["value"] = damage;

    
    local aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",radius,0,0,true,Bor(ATTACK_HITS_FRIENDLIES, ATTACK_HITS_ENEMIES),COLOR_DAMAGE,DITHER_HEAL_HALF,false,-1,0,0,0,{f1});
    SetAttackInactive(aoe,seconds)

    local dist = DistXY(GetX(GetObjRef()),GetY(GetObjRef()),x,y)

    local heading = GetHeadingVector(GetX(GetObjRef()) - x, GetY(GetObjRef()) - y)
    
    SetAttackMoveAngle(aoe,heading.headingx,heading.headingy)
    SetAttackVelocity(aoe, dist / seconds)  

    return true; 
end
function ontimeout()
    PlaySound("assets/friendly/shaman/audio/spirit_timeout.wav",1);
    ShakeScreen(5,0.1)
end
function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
end