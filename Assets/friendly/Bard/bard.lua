function setup()
    SetSprite("Assets/friendly/Bard/bard.png");
    AddAbility("Assets/friendly/Bard/testability.lua",0)    
    AddAbility("Assets/friendly/Bard/testblink.lua",1)    

    AddAttackSprite("Assets/UI/slash_fx3.png",16,16,0.05);
    AddAttackSprite("Assets/UI/slash_fx2.png",16,16,0.05);
    AddAttackSprite("Assets/UI/slash_fx.png",32,32,0.05);


end

function update()
end

function kill()

end