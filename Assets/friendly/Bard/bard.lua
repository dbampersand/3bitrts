function setup()
    SetSprite("Assets/friendly/Bard/bard.png");
    AddAbility("Assets/friendly/Bard/aoe_speed.lua",0)    
    AddAbility("Assets/friendly/Bard/aoe_damage.lua",1)    
    AddAbility("Assets/friendly/Bard/heal.lua",2)    
    AddAbility("Assets/friendly/Bard/aoeheal.lua",3)    

    AddAttackSprite("Assets/UI/slash_fx3.png",16,16,0.05);
    AddAttackSprite("Assets/UI/slash_fx2.png",16,16,0.05);
    AddAttackSprite("Assets/UI/slash_fx.png",32,32,0.05);

    SetDamage(50);

end

function update()
end

function kill()

end