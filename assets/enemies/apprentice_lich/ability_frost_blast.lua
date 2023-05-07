local damage = 120

local duration = 4

function setup()

    SetAbilityRange(200)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(6); 
    AbilitySetPortrait("assets/enemies/skeleton_basher/ability_bash.png");
    SetDescription("[b]Frost Blast\n\nPushes away a target and slows them.")
end
function casted(x,y,obj,headingx,headingy)

    if (ObjIsValidIndex(obj) == false) then
        return false;
    end

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;

    local f2 = {};
    f2["trigger"] = TRIGGER_CONST;
    f2["type"] = EFFECT_SPEED;
    f2["value"] = -30;
    f2["duration"] = duration


    PushObj(GetX(GetObjRef()),GetY(GetObjRef()),obj,600,0.1);

    ApplyEffect(obj,{f1,f2});

    ShakeScreen(3,0.2)

    PlaySound("assets/enemies/viper/audio/bite.wav",1);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end