local damage = 120

local duration = 4

function setup()

    SetAbilityRange(200)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(8); 
    AbilitySetPortrait("assets/enemies/apprentice_lich/ability_frost_blast.png");
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


    PushObj(GetX(GetObjRef()),GetY(GetObjRef()),obj,300,0.1);

    ApplyEffect(obj,{f1,f2});

    ShakeScreen(3,0.2)

    PlaySound("assets/enemies/apprentice_lich/audio/frost_blast.wav",0.5);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end