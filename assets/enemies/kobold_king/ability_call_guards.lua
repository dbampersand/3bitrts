function setup()
    SetAbilityRange(120)
    SetCooldown(60);
    AbilitySetPortrait("assets/enemies/cusith/ability_focus.png");
    SetDescription("[b]Call Guards\n\nCalls the royal guards.")
    
end

function casted(x,y,obj,headingx,headingy)



    CreateObject("assets/enemies/royal_guard/royal_guard.lua",x,y,TYPE_FRIENDLY,3);
    CreateObject("assets/enemies/royal_guard/royal_guard.lua",x,y,TYPE_FRIENDLY,3);

    return true;
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target, attack)
end

function abilitytick(x, y, timeUntilAttackTick, parent, target, dt, attackRef, totalAttackDurationLeft)

end