
function setup()
    SetAbilityRange(120)
    SetCooldown(12);
    AbilitySetPortrait("assets/enemies/cannon/ability_shoot.png");
    SetDescription("[b]Shoot\n\nDeals damage in an area.");
    SetAbilityName("Crash"); 
    SetAbilityHint(HINT_CIRCLE,25);

end

function casted(x,y,obj,headingx,headingy)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 140;

    CreateAOE(x,y,"", 25, 0, 0, false, Bor(ATTACK_HITS_ENEMIES,ATTACK_HITS_FRIENDLIES), COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, false, obj, {f1,f2})
    PlaySound("assets/enemies/cannon/audio/cannon_fire.wav",0.5)

    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/cannon/cannon_cast_shoot.png")
    PlaySound("assets/enemies/cannon/audio/cannon_onchannel.wav",0.5)
end