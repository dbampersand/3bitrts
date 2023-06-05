local radius = 60
local aoe = -1;

local lifestealPercent = 0.4

local damage = 150
function setup()
    AbilitySetCastType(ABILITY_INSTANT);

    SetAbilityRange(20)
    SetCooldown(99999999)
    SetCooldownTimer(99999999)


    SetAbilityMaxStacks(3)
    SetAbilityStacks(3)

    AbilitySetPortrait("assets/friendly/bard/icon_aoe_heal.png");
    SetDescription("Weave\n\nCreates a creature, based on the number of stacks on Weave.")

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/bard/audio/song_of_healing.wav",1)
    local obj = -1
    if (GetStacks(GetObjRef(),GetAbilityRef())==1) then
        obj = CreateObject("assets/summoned/woven_imp/woven_imp.lua",GetX(GetObjRef()),GetY(GetObjRef()),TYPE_FRIENDLY,0);
    end
    if (GetStacks(GetObjRef(),GetAbilityRef())==2) then
        obj = CreateObject("assets/summoned/woven_dove/woven_dove.lua",GetX(GetObjRef()),GetY(GetObjRef()),TYPE_FRIENDLY,0);
    end
    if (GetStacks(GetObjRef(),GetAbilityRef())==3) then
        obj = CreateObject("assets/summoned/woven_demon/woven_demon.lua",GetX(GetObjRef()),GetY(GetObjRef()),TYPE_FRIENDLY,0);
    end

    SetLifetime(obj,12)
    AddStack(GetObjRef(),2,-3)
    AddStack(GetObjRef(),3,-3)

    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
end

function effecttick(effectType,value,obj,x,y)
end 