local totem = -1

function setup()
    AbilitySetPortrait("assets/friendly/shaman/icon_swift_totem.png");
    SetDescription("Swift Totem\n\nCasts a movement speed aura, also increases attack speed. Only one totem can be active.")

    AbilitySetCastType(ABILITY_INSTANT);

    SetCooldown(2);
end


function casted(x,y,obj,headingx,headingy)
    UntoggleAll()
    totem = CreateObject("assets/friendly/shaman/totems/swift_totem.lua",GetX(GetObjRef()),GetY(GetObjRef()));
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