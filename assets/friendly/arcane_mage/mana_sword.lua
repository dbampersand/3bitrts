local duration = 25
local timer = 0
local ticker = -1
function setup()
    AbilitySetPortrait("assets/friendly/arcane_mage/icon_mana_sword.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Mana Sword\n\nSummon a magic sword, changing all of your abilities and changing your range to melee.")
    SetCooldown(0);
    SetAbilityRange(50);

    SetCooldown(20);
    SetManaCost(5);

end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/arcane_mage/audio/mana_sword.wav",1)

    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/mana_sword/slash.lua",0)
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/mana_sword/empower.lua",1)
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/mana_sword/dash.lua",2)
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/mana_sword/throw_sword.lua",3)


    f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ATTACKRANGE;
    f1["value"] = -100;
    f1["duration"] = duration
    f1["name"] = "Mana Sword"

    ApplyEffect(GetObjRef(),{f1})

    ticker = CreateTicker(duration);

   return false;
end
function ontimeout(x,y,obj,dt,target)

    AddAbility(obj,"assets/friendly/arcane_mage/bolt.lua",0)
    AddAbility(obj,"assets/friendly/arcane_mage/teleport.lua",1)
    AddAbility(obj,"assets/friendly/arcane_mage/arcane_wave.lua",2)
    AddAbility(obj,"assets/friendly/arcane_mage/mana_sword.lua",3);
end
function onhit(x,y,objhit)
end



function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    timer = timer + dt

end