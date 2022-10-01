local adds = {}


function setup()
    SetAbilityRange(999)
    SetCooldown(15)
    AbilitySetPortrait("assets/enemies/minotaur/ability_charge.png");
    SetDescription("[b]Call\n\nCalls for help, summoning extra enemies.")
    SetAbilityHint(HINT_CIRCLE);

    adds[1] = "assets/enemies/berzerker/berzerker.lua"
    adds[2] = "assets/enemies/shield_warrior/shield_warrior.lua"
    adds[3] = "assets/enemies/drummer/drummer.lua"
    
end

function casted(x,y,obj,headingx,headingy)
    local toSpawn = math.random(#adds);
    CreateObject(adds[toSpawn],x,y,TYPE_ENEMY,1);
    SetObjChannelling(GetObjRef(),1);

    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)
end


function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)

end