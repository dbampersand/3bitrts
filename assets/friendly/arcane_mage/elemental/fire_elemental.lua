local maxHP = 150;
local lifetime = 20

function setup()
    SetSprite("assets/friendly/arcane_mage/elemental/fire_elemental.png");

    AddAbility(GetObjRef(),"assets/friendly/arcane_mage/elemental/burnout.lua",0)


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(20);
    SetRange(5)
    SetSpeed(60)
    SetMaxHP(maxHP,true);

    SetObjType(TYPE_RANGEDDPS);
    SetCategory(TYPE_RANGEDDPS);
    
    SetObjectPush(false);
end

function update(dt)
    local hpRemove = maxHP / lifetime * dt;
    HurtObj(GetObjRef(),hpRemove);
end

function kill()
end