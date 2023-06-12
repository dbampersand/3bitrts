local range = 30

function setup()

    SetAbilityRange(10)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_CIRCLE,30);
    SetCooldown(15); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Acid\n\nQuickly deals damage.")
end
function onchanneled()
    SetChannelingSprite("assets/enemies/giant_ooze/giant_ooze_casting_acid.png");
end

function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT_PERCENT;
    f1["value"] = 0.01;
    f1["name"] = "Acid"
    f1["overwrites"] = true

     CreateAOE(GetX(obj),GetY(obj),"",40,0.1,30,false,ATTACK_HITS_ENEMIES,COLOR_POISON,DITHER_POISON_QUARTER,false,-1,{f1});
     PlaySound("assets/enemies/giant_ooze/audio/acid.wav",0.75);

    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end