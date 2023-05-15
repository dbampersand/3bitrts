
function setup()
    AbilitySetPortrait("assets/friendly/arcane_mage/mana_sword/icon_throw_sword.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetDescription("Throw Sword\n\nThrows the mana sword at a target, dealing damage and returning your previous abilities.");
    SetCooldown(8);
    SetAbilityRange(50);    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/cloak.wav",1.25)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 40;
    f1["duration"] = 6


    CureNamedEffect(GetObjRef(), "Mana Sword", 1);
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/bolt.lua",0)
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/teleport.lua",1)
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/arcane_wave.lua",2)
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/mana_sword.lua",3);

    ApplyEffect(obj,{f1});
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end