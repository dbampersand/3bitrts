local time = 8

function setup()
    SetAbilityRange(32)
    SetCooldown(24);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Melt\n\nDestroys target's armour, causing them to take more damage.");
    SetAbilityName("Melt"); 

end

function casted(x,y,obj,headingx,headingy)
    

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST; 
    f1["type"] = EFFECT_ARMOR;
    f1["value"] = -10;
    f1["duration"] = time;

    ApplyEffect(obj,{f1});

    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
end