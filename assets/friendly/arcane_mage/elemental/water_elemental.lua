local maxHP = 100;

function setup()
    SetSprite("assets/friendly/arcane_mage/elemental/water_elemental.png");

    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/elemental/tsunami.lua",0)


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(15);
    SetRange(5)
    SetSpeed(60)
    SetMaxHP(maxHP,true);

    SetObjType(TYPE_RANGEDDPS);
    SetCategory(TYPE_RANGEDDPS);
    
    SetObjectPush(false);
    SetLifetime(GetObjRef(),15);

end

function update(dt)

end

function kill()
end