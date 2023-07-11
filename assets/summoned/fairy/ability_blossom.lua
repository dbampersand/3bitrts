local heal = 50

function setup()
    SetAbilityRange(256) 
    SetCooldown(20);
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetHintColor(EFFECT_HEAL)

    AbilitySetPortrait("assets/summoned/fairy/ability_blossom.png");
    SetDescription("[b]Blossom\n\nThrows blossoms, healing units when it touches them.");
    SetAbilityName("Blossom"); 

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/summoned/fairy/audio/blossom.wav",1);
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    f1["value"] = heal;

    CreateCircularProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),"",ATTACK_PROJECTILE_ANGLE,80,999,false,ATTACK_HITS_FRIENDLIES,16,COLOR_HEAL,0,{f1})
    return true; 
end

function onhit(x,y,objhit)
    PlaySound("assets/summoned/fairy/audio/blossom_hit.wav",1);
end
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
end
function onchanneled() 
end