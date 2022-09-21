local totem = -1

function setup()
    AbilitySetPortrait("assets/friendly/shaman/icon_stoneskin_totem.png");
    SetDescription("Stoneskin Totem\n\nCasts a circle of protection, giving armor to those inside. Only one totem can be active.")

    AbilitySetCastType(ABILITY_INSTANT);

    SetCooldown(2);
end


function casted(x,y,obj,headingx,headingy)
    UntoggleAll()
    totem = CreateObject("assets/friendly/shaman/totems/stoneskin_totem.lua",GetX(GetObjRef()),GetY(GetObjRef()));
    return true; 
end

function untoggle()
    KillObj(totem);
    totem = -1;
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
end