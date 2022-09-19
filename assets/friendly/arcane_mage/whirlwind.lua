local radius = 60;

function setup()
    AbilitySetPortrait("assets/friendly/monk/icon_whirlwind.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityRange(50)
    SetDescription("Whirlwind\n\nUses a chi to deal area damage to all nearby enemies.")

    SetCooldown(99999999);
    SetCooldownTimer(99999999);
    SetAbilityMaxStacks(5);
    SetAbilityStacks(0)

end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/shiv.wav",1.25)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 100;  

    local aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius, 0.25, 0.25, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_STAR_HALF,false, -1, {f1});

    AddStack(GetObjRef(),2,-1);
    AddStack(GetObjRef(),4,-1);

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end