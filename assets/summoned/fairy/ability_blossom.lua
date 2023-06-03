local heal = 50

function setup()
    SetAbilityRange(256) 
    SetCooldown(20);
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Blossom\n\nThrows blossoms, healing units when it touches them.");
    SetAbilityName("Blossom"); 

end

function casted(x,y,obj,headingx,headingy)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    f1["value"] = heal;

    CreateCircularProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),"",ATTACK_PROJECTILE_ANGLE,80,999,false,ATTACK_HITS_FRIENDLIES,16,COLOR_HEAL,0,{f1})
    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
    aoe = -1;
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
end
function onchanneled() 
end