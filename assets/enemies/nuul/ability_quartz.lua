local time = 8

function setup()
    SetAbilityRange(32)
    SetCooldown(24);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Quartz\n\nCreates an area of damaging fire.");
    SetAbilityName("Melt"); 

end

function casted(x,y,obj,headingx,headingy)
    

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT; 
    f1["type"] = EFFECT_HURT;
    f1["value"] = 100;

    CreateAttackArea({{14,15},{50,50},{80,30}},0,0,"",0.5,5,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,0,false);

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