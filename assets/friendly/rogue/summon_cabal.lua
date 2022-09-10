
function setup()
    AbilitySetPortrait("assets/friendly/rogue/icon_summon_cabal.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Summon Cabal\n\nSummons two minions and increases damage around you.")
    SetCooldown(60);
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/summon.wav",1.25)

    local xpos = GetX(GetObjRef());
    local ypos = GetY(GetObjRef());



    CreateObject("assets/friendly/rogue/cabal/cabal.lua",xpos,ypos,TYPE_FRIENDLY,0.5);
    CreateObject("assets/friendly/rogue/cabal/cabal.lua",xpos,ypos,TYPE_FRIENDLY,0.5);

   return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end