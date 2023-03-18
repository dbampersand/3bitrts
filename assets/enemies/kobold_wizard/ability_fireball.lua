local length = 15
local time = 12

function setup()
    SetAbilityRange(256)
    SetCooldown(20);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Fireball\n\nThrows a fireball towards the target, exploding after " .. time .. " seconds");
    SetAbilityName("Fireball"); 
    SetAbilityHint(HINT_LINE);

end

function casted(x,y,obj,headingx,headingy)
    

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 80;

    CreateProjectile(GetX(GetObjRef()),GetY(GetObjRef()),GetX(obj),GetY(obj),"",ATTACK_PROJECTILE_POINT,25,4,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,{f1})


    return true; 
end

local function makeAOE(x,y)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 30;

     CreateAOE(x,y,"", 50, 1, 6, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_EIGTH, false, -1, {f1})
end
function onhit(x,y,objhit)
    makeAOE(x,y)
end
function ontimeout(x,y,obj,dt,target)
    makeAOE(x,y)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
end