local maxHP = 150;

function setup()
    SetSprite("assets/friendly/priest/priest.png");
    AddAbility("assets/friendly/priest/smite.lua",0)    
    AddAbility("assets/friendly/priest/minorheal.lua",1)    
    AddAbility("assets/friendly/priest/greaterheal.lua",2)    
    AddAbility("assets/friendly/priest/shield.lua",3)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(5);
    SetRange(100)
    SetMaxHP(maxHP,true)
    SetManaRegen(5);

    SetObjType(TYPE_HEALER);
    IsPlayerChoosable(true);





end

function update(dt)
end

function kill()

end