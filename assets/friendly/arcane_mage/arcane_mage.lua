local maxHP = 250;
local heal = -1

function setup()
    SetSprite("assets/friendly/arcane_mage/arcane_mage.png");

    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/bolt.lua",0)
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/teleport.lua",1)
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/summon_elemental.lua",2)
    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/mana_sword.lua",3);


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(15);
    SetRange(50)
    SetSpeed(60)
    SetMaxHP(maxHP,true);
    ObjectUsesMana(GetObjRef(),true)

    SetObjType(TYPE_RANGEDDPS);

    IsPlayerChoosable(true);
    SetCategory(TYPE_RANGEDDPS);
    
    SetObjectPush(false);
end

function update(dt)

end

function kill()
end