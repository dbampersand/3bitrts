function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/cusith/ability_call_pack.png");
    SetDescription("[b]Call Pack\nSummons adds");
    SetCooldown(40)

end

function casted(x,y,obj,headingx,headingy)

    CreateObject("assets/enemies/wolf/wolf.lua",128,128,TYPE_FRIENDLY,2);
    CreateObject("assets/enemies/wolf/wolf.lua",128,128,TYPE_FRIENDLY,2);
    CreateObject("assets/enemies/wolf/wolf.lua",128,128,TYPE_FRIENDLY,2);

    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end