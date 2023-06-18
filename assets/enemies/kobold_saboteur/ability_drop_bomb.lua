function setup()
    SetAbilityRange(256)
    SetCooldown(3.5);
    SetAbilityMaxStacks(5)
    AbilitySetPortrait("assets/enemies/kobold_saboteur/ability_drop_bomb.png");
    SetDescription("[b]Drop Bomb\n\nDrops a bomb.")
    SetAbilityName("Drop Bomb"); 
    SetAbilityHint(HINT_CIRCLE,20);

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/kobold_saboteur/audio/drop_bomb.wav",0.5)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 60;  


    CreateAOE(x,y,"", 25, 1.25, 1.25, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_HALF, false, obj, {f1})

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

function applyattack()
    PlaySound("assets/enemies/kobold_saboteur/audio/drop_bomb_apply.wav",0.5)
end