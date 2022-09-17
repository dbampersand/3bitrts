local maxHP = 250;

function setup()
    SetSprite("assets/friendly/priest/priest.png");
    AddAbility("assets/friendly/priest/smite.lua",0)    
    AddAbility("assets/friendly/priest/circleofhealing.lua",1)    
    AddAbility("assets/friendly/priest/regen.lua",2)    
    AddAbility("assets/friendly/priest/shield.lua",3)    
    AddAbility("assets/friendly/priest/greaterheal.lua",4)    


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