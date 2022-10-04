function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_summon_adds.png");
    SetDescription("[b]Call Pack\nSummons adds");
    SetCooldown(40)

end

function casted(x,y,obj,headingx,headingy)

    CreateObject("assets/enemies/wolf/wolf.lua",128,128,GetOwnedBy(GetObjRef()),2);
    CreateObject("assets/enemies/wolf/wolf.lua",128,128,GetOwnedBy(GetObjRef()),2);
    CreateObject("assets/enemies/wolf/wolf.lua",128,128,GetOwnedBy(GetObjRef()),2);
    CreateObject("assets/enemies/wolf/wolf.lua",128,128,GetOwnedBy(GetObjRef()),2);
    CreateObject("assets/enemies/wolf/wolf.lua",128,128,GetOwnedBy(GetObjRef()),2);

    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end