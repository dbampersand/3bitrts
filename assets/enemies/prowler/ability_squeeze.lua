local range = 30

function setup()

    SetAbilityRange(10)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(15); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Squeeze\n\nDeals a third of the health of units in an area.")
end
function onchanneled()
    SetChannelingSprite("assets/enemies/giant_ooze/giant_ooze_channelling.png");
end

function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["duration"] = 0.2
    f1["triggersPerSecond"] = 1
    f1["type"] = EFFECT_HURT_PERCENT;
    f1["value"] = 0.34;  
    f1["name"] = "Squeeze"
    f1["overwrites"] = true

    CreateCircularProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),"",ATTACK_PROJECTILE_ANGLE,80,999,false,ATTACK_HITS_ENEMIES,128,COLOR_DAMAGE,0,{f1})
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end