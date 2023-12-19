local range = 30

function setup()

    SetAbilityRange(10)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/wolf/ability_pack_animal.png");
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
    PlaySound("assets/enemies/wolf/audio/pack_animal.wav",0.35);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end