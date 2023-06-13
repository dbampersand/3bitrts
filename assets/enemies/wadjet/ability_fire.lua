function setup()

    SetAbilityRange(100)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/wadjet/ability_fire.png");
    SetDescription("[b]Sun Disc\n\nPlaces an area of effect, dealing damage over time.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 15;  
    f1["portrait"] = "assets/enemies/wyrm_boss/ability_fire_effect_portrait"
    PlaySound("assets/enemies/wyrm_boss/audio/fire.wav",1)
    
    CreateCircularProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),"",ATTACK_PROJECTILE_ANGLE,60,10,true,ATTACK_HITS_ENEMIES,12,COLOR_FRIENDLY_DAMAGE,0,{f1})

    PlaySound("assets/enemies/wadjet/audio/fire.wav",1);

    return true; 
end

function onhit(x,y,objhit)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 15;  

    CreateAOE(x,y,"", 30, 1, 999, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_HALF,false,-1, {f1})
end

function abilitytick(x, y, durationLeft)
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/wadjet/naja_cast_fire.png")
end