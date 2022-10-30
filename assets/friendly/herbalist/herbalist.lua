local maxHP = 250;

function setup()
    SetSprite("assets/friendly/herbalist/herbalist.png");
    AddAbility(GetObjRef(),"assets/friendly/herbalist/elixir_of_life.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/herbalist/regenerative_herbs.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/herbalist/panacea.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/herbalist/dissolve.lua",3)    
    AddAbility(GetObjRef(),"assets/friendly/herbalist/mix.lua",4)    

    AddItem(GetObjRef(),"assets/items/quality_mid/hermes_boots/hermes_boots.lua");

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(5);
    SetRange(100)
    SetMaxHP(maxHP,true)
    SetManaRegen(15);

    SetObjType(TYPE_HEALER);
    IsPlayerChoosable(true);

    SetCategory(TYPE_HEALER);

    ObjectUsesMana(GetObjRef(),true);



end

function update(dt)
end

function kill()

end