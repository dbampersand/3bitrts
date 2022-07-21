local length = 10

function setup()
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY); 
    AbilitySetPortrait("assets/friendly/chromamancer/icon_illusion.png");
    SetDescription("Illusion\n\nCopies the target friendly unit for "..length.." seconds.")

    SetAbilityRange(20)
    SetCooldown(20);

end

function casted(x,y,obj,headingx,headingy)
    copied = CopyObject(obj,GetX(GetObjRef()),GetY(GetObjRef()));
    SetLifetime(copied,5);
    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
end