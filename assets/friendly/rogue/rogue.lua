local maxHP = 300;

function setup()
    SetSprite("assets/friendly/rogue/rogue.png");

    AddAbility(GetObjRef(),"assets/friendly/rogue/shiv.lua",0)
    AddAbility(GetObjRef(),"assets/friendly/rogue/cloak.lua",1)
    AddAbility(GetObjRef(),"assets/friendly/rogue/backstab.lua",2)
    AddAbility(GetObjRef(),"assets/friendly/rogue/toxic.lua",3)
    AddAbility(GetObjRef(),"assets/friendly/rogue/summon_cabal.lua",4)


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(15);
    SetRange(1)
    SetSpeed(60)
    SetMaxHP(maxHP,true);

    SetObjType(TYPE_MELEEDPS);

    IsPlayerChoosable(true);    
    SetCategory(TYPE_MELEEDPS);
    
    SetObjectPush(false);

    AddItem(GetObjRef(),"assets/items/barbed_wire/barbed_wire.lua");
end

function update(dt)
end

function kill()

end