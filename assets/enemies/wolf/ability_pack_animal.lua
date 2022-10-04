local range = 30

function setup()

    SetAbilityRange(10)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Pack Animal\n\nIncreases damage based on how many friendly units are nearby.")
end
function casted(x,y,obj,headingx,headingy)

    local numFriendliesInRange = GetObjectsInRange(GetX(GetObjRef()),GetY(GetObjRef()),30,GetOwnedBy(GetObjRef()));
    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ATTACKDAMAGE;
    f1["value"] = 2 * numFriendliesInRange;
    f1["duration"] = 5;


    ApplyEffect(GetObjRef(), {f1});
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end