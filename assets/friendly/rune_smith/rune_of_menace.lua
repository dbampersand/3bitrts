local duration = 3

function setup()
    AbilitySetPortrait("assets/friendly/rune_smith/icon_rune_of_menace.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetCooldown(5);

    SetDescription("Rune of Menace\n\nCreates an area of threat around you.")
    SetCooldown(16)

end

function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_THREAT;
    f1["value"] = 1200;  


    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",90,0.1,0.1,false,ATTACK_HITS_ENEMIES,COLOR_FRIENDLY_DAMAGE,DITHER_NONE,false,-1,{f1});
    
    PlaySound("assets/friendly/rune_smith/audio/rune_of_menace.wav",1);

    return true;
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end