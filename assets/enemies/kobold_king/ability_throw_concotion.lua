function setup()
    SetAbilityRange(999)
    SetCooldown(20)
    SetAbilityHint(HINT_CIRCLE,30,true);
    AbilitySetCastType(ABILITY_POINT);
    AbilitySetPortrait("assets/enemies/kobold_king/ability_throw_concoction.png");
    SetDescription("[b]Throw Concoction\n\nDeals a large amount of damage to all units in an area.")
    SetAbilityName("Throw Concoction"); 

end

function casted(x,y,obj,headingx,headingy)


    PlaySound("assets/enemies/kobold_king/audio/throw_concoction.wav",1)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 40;  


    CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 15, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, false, obj,0,0,0,{f1})
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
    
end

function onchanneled() 
    SetChannelingSprite("assets/enemies/kobold_king/kobold_king_chanelling_throw_concotion.png");
end

function applyattack(a, x, y)
    PlaySound("assets/enemies/kobold_king/audio/throw_concoction_tick.wav",1,x,y)
end 