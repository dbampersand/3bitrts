function setup()
    SetAbilityRange(30)
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Summon\n\nSummons three vipers.")
    AbilitySetCastType(ABILITY_INSTANT);
    SetMaxHP(200);
end
function casted(x,y,obj,headingx,headingy)
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_CURE;
    f1["value"] = 1;

    local aoe = CreateAOE(x,y,"",120,0.25,1,false,ATTACK_HITS_ENEMIES,COLOR_HEAL,DITHER_VERTICAL_HALF,false,-1,{f1});
    SetAttackCircle(aoe,true);

    PlaySound("assets/enemies/caduceus/audio/cure_aoe.wav",0.7);

    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end