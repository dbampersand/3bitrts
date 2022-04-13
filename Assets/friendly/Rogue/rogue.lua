function setup()
    SetSprite("Assets/friendly/Rogue/rogue.png");
    AddAbility("Assets/friendly/Rogue/shiv.lua",0)    
    AddAbility("Assets/friendly/Rogue/dash.lua",1)    
    AddAbility("Assets/friendly/Rogue/backstab.lua",2)    
    AddAbility("Assets/friendly/Rogue/cloak.lua",3)    

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