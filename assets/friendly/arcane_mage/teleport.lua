
function setup()
    AbilitySetPortrait("assets/friendly/arcane_mage/icon_teleport.png");
    AbilitySetCastType(ABILITY_POINT);
    SetAbilityRange(50)
    SetDescription("Teleport\n\nMove to target position, increasing your and allies' attack speed.")
    SetCooldown(15);
    SetManaCost(20);

end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/arcane_mage/audio/teleport.wav",1)
    
    f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_ATTACKSPEED
    f1["value"] = -0.1
    f1["duration"] = 6

    Teleport(GetObjRef(),x,y);
    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 30, 0.25, 0.25, false, ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_DAMAGE,DITHER_EIGTH,false, -1,0,0,0, {f1})
    ShakeScreen(1.5,0.1)    


    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end