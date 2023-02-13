local range = 30

function setup()

    SetAbilityRange(10)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/explosive_bomb/ability_explode.png");
    SetDescription("[b]Explode\n\nExplodes, dealing massive damage to all enemy units.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 75;

    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",999,1,1,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_EIGTH,false,-1,{f1});
    KillObj(GetObjRef());
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end