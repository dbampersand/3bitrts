
function setup()

    SetAbilityRange(999)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_CIRCLE,60);
    SetCooldown(15); 
    AbilitySetPortrait("assets/enemies/royal_guard/ability_charge.png");
    SetDescription("[b]Charge\n\nDashes to a random unit, then dealing damage in an area around them.")
end
function casted(x,y,obj,headingx,headingy)


    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 50;


    Teleport(GetObjRef(),GetX(obj),GetY(obj));
    SetObjChannelling(GetObjRef(),1.25);

    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",1.25,1.25,60,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,{f1});
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end