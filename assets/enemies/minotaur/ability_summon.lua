local adds = {}


function setup()
    SetAbilityRange(999)
    SetCooldown(15)
    AbilitySetPortrait("assets/enemies/minotaur/ability_summon_adds.png");
    SetDescription("[b]Call\n\nCalls for help, summoning extra enemies.")
    SetAbilityHint(HINT_CIRCLE);

    adds[1] = "assets/enemies/berzerker/berzerker.lua"
    adds[2] = "assets/enemies/shield_warrior/shield_warrior.lua"
    adds[3] = "assets/enemies/firebreather/firebreather.lua"
    
end

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/enemies/minotaur/audio/summon.wav",1)

    local toSpawn = Round(RandRange(1,#adds));
    CreateObject(adds[toSpawn],x,y,TYPE_FRIENDLY,1);
    SetObjChannelling(GetObjRef(),1);

    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)
end


function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)

end