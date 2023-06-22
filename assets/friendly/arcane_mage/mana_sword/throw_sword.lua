
function setup()
    AbilitySetPortrait("assets/friendly/arcane_mage/mana_sword/icon_throw_sword.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetDescription("Throw Sword\n\nThrows the mana sword at a target, dealing damage and returning your previous abilities.");
    SetCooldown(8);
    SetAbilityRange(50);    
end
function casted(x,y,obj,headingx,headingy)
    Print("1")

    PlaySound("assets/friendly/arcane_mage/audio/throw_sword.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 40;
    f1["duration"] = 6

    Print("1")
    CureNamedEffect(GetObjRef(), "Mana Sword", 1);
    Print("2")
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/bolt.lua",0)
    Print("3")
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/teleport.lua",1)
    Print("4")
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/arcane_wave.lua",2)
    Print("5")
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/mana_sword.lua",3);
    Print("6")

    ApplyEffect(obj,{f1});
    Print("7")
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end