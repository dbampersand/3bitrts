function setup()

    SetAbilityRange(999)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(6); 
    SetAbilityMaxStacks(3);
    AbilitySetPortrait("assets/enemies/wadjet/ability_sun_disc.png");
    SetDescription("[b]Sun Disc\n\nPlaces an area of effect, dealing damage over time.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 5;  
    f1["portrait"] = "assets/enemies/wyrm_boss/ability_fire_effect_portrait"

    local atk = CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 60, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HALF,false,-1, {f1})
    SetAttackInactive(atk,6)
    PlaySound("assets/enemies/wadjet/audio/sun_disc.wav",1);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/wadjet/wadjet_cast_sun_disc.png")
end