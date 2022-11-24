function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_summon_adds.png");
    SetDescription("[b]Summon\nSummons adds");
    SetCooldown(30)

end

function casted(x,y,obj,headingx,headingy)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    --f1["triggersPerSecond"] = 1 
    f1["value"] = 20;  
    --f1["duration"] = 4;

    CreateObject("assets/enemies/wyrm_boss/wyrm_add/wyrm_add.lua",128,128,TYPE_FRIENDLY,2);
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end