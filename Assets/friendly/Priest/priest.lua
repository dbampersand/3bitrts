function setup()
    SetSprite("Assets/friendly/priest/priest.png");
    AddAbility("Assets/friendly/priest/smite.lua",0)    
    AddAbility("Assets/friendly/priest/minorheal.lua",1)    
    AddAbility("Assets/friendly/priest/greaterheal.lua",2)    
    AddAbility("Assets/friendly/priest/shield.lua",3)    

    AddAttackSprite("Assets/UI/slash_fx3.png",16,16,0.05);
    AddAttackSprite("Assets/UI/slash_fx2.png",16,16,0.05);
    AddAttackSprite("Assets/UI/slash_fx.png",32,32,0.05);   

    SetDamage(5);
    SetRange(100)




end

function update()
end

function kill()

end