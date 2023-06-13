
function setup()
    SetAbilityRange(32)
    SetCooldown(24);
    AbilitySetPortrait("assets/enemies/kobold_crossbow/ability_tar_trap.png");
    SetDescription("[b]Tar Trap\n\nSlows enemies in a radius.");
    SetAbilityName("Tar Trap"); 

end

function casted(x,y,obj,headingx,headingy)
    

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST; 
    f1["type"] = EFFECT_SPEED;
    f1["value"] = -30;
    f1["duration"] = 1.5;
    f1["overwrites"] = true;
    f1["name"] = "Tar Trap";

    CreateAOE(x,y,"", 45, 0.1, 12, false, ATTACK_HITS_ENEMIES, COLOR_SPEED, DITHER_HORIZONTAL_QUARTER, false, obj, {f1})

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