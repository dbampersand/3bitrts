function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_summon_adds.png");
    SetDescription("[b]Summon\nSummons adds");
    SetCooldown(30)

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/wyrm_boss/audio/summon_add.wav",1)

    CreateObject("assets/enemies/wyrm_boss/wyrm_add/wyrm_add.lua",128,128,TYPE_FRIENDLY,2);
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end