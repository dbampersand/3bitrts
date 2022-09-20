function setup()
    SetSprite("assets/friendly/paladin/paladin.png");
    AddAbility(GetObjRef(),"assets/friendly/paladin/shield_slam.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/paladin/holy_word.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/paladin/circle_of_protection.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/paladin/lay_on_hands.lua",3)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(12);

    SetObjType(TYPE_TANK)
    SetThreatMultiplier(6);
    IsPlayerChoosable(true);
    SetCategory(TYPE_TANK);

    SetMaxHP(1000,true);
    
    ObjectUsesMana(GetObjRef(),true);

end

function update(dt)
end

function kill()

end