function setup()
    SetSprite("Assets/friendly/warrior/warrior.png");
    AddAbility("Assets/friendly/warrior/testability.lua",0)    
    AddAbility("Assets/friendly/warrior/testblink.lua",1)    
    AddAbility("Assets/friendly/warrior/heal.lua",2)    
    AddAbility("Assets/friendly/warrior/aoeheal.lua",3)    

    AddAttackSprite("Assets/UI/slash_fx3.png",16,16,0.05);
    AddAttackSprite("Assets/UI/slash_fx2.png",16,16,0.05);
    AddAttackSprite("Assets/UI/slash_fx.png",32,32,0.05);   


end

function update()
end

function kill()

end