function setup()
    SetSprite("assets/friendly/rune_smith/rune_smith.png");
    AddAbility(GetObjRef(),"assets/friendly/rune_smith/rune_of_lifesteal.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/rune_smith/rune_of_pain.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/rune_smith/rune_of_protection.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/rune_smith/rune_of_shield.lua",3)    
    AddAbility(GetObjRef(),"assets/friendly/rune_smith/rune_of_menace.lua",4)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(12);

    SetObjType(TYPE_TANK)
    SetThreatMultiplier(12);
    IsPlayerChoosable(true);
    SetCategory(TYPE_TANK);

    SetMaxHP(1000,true);

end

function update(dt)
end

function kill()

end