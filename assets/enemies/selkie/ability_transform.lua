function setup()

    SetAbilityRange(10)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(9999); 
    AbilitySetPortrait("assets/enemies/selkie/ability_transform.png");
    SetDescription("[b]Transform\n\nTransforms into a seal, increasing attack damage and speed.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ATTACKDAMAGE;
    f1["value"] = 15;
    f1["duration"] = 999;

    local f2 = {};
    f2["trigger"] = TRIGGER_CONST;
    f2["type"] = EFFECT_ATTACKSPEED;
    f2["value"] = -0.25;
    f2["duration"] = 999;


    ApplyEffect(GetObjRef(), {f1,f2});
    SetSprite("assets/enemies/selkie/selkie_transformed.png")
    SetSprite("assets/enemies/selkie/selkie_transformed.png")

    PlaySound("assets/enemies/wolf/audio/call_pack.wav",0.35);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end