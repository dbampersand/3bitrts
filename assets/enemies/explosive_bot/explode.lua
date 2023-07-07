local range = 30

function setup()

    SetAbilityRange(10)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/explosive_bot/ability_explode.png");
    SetDescription("[b]Explode\n\nExplodes, dealing massive damage to all enemy units.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 400;

    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",128,1,1,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_EIGTH,false,-1,0,0,0,{f1});
    KillObj(GetObjRef());

    After(PlaySound,1,true,"assets/enemies/explosive_bot/audio/explode.wav",3,GetX(GetObjRef()),GetY(GetObjRef()));
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end