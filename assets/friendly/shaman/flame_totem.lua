local totem = -1

function setup()
    AbilitySetPortrait("assets/friendly/shaman/icon_flame_totem.png");
    SetDescription("Fire Totem\n\nCasts a damaging aura. Only one totem can be active.")

    AbilitySetCastType(ABILITY_INSTANT);

    SetCooldown(2);
end


function casted(x,y,obj,headingx,headingy)
    UntoggleAll()
    totem = CreateObject("assets/friendly/shaman/totems/fire_totem.lua",GetX(GetObjRef()),GetY(GetObjRef()));
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