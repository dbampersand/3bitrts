function setup()
    SetAbilityRange(30)
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/caduceus/ability_cureaoe.png");
    SetDescription("[b]Cure\n\nCures all targets in an area.")
    AbilitySetCastType(ABILITY_INSTANT);
end
function casted(x,y,obj,headingx,headingy)
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_CURE;
    f1["value"] = 3;

    local aoe = CreateAOE(x,y,"",120,0.25,1,false,ATTACK_HITS_ENEMIES,COLOR_HEAL,DITHER_HEAL_EIGTH,false,-1,{f1});
    SetAttackCircle(aoe,true);

    PlaySound("assets/enemies/caduceus/audio/cure_aoe.wav",0.7);

    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/caduceus/caduceus_cast_cure_aoe.png")
end