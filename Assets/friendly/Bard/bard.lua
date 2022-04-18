function setup()
    SetSprite("Assets/friendly/Bard/bard.png");
    AddAbility("Assets/friendly/Bard/aoe_speed.lua",0)    
    AddAbility("Assets/friendly/Bard/aoe_damage.lua",1)    
    AddAbility("Assets/friendly/Bard/aoe_shield.lua",2)    
    AddAbility("Assets/friendly/Bard/aoe_heal.lua",3)    

    AddAttackSprite("Assets/UI/slash_fx3.png",16,16,0.05);
    AddAttackSprite("Assets/UI/slash_fx2.png",16,16,0.05);
    AddAttackSprite("Assets/UI/slash_fx.png",32,32,0.05);

    SetDamage(5);
    SetRange(30);

    SetObjType(Bor(TYPE_HEALER,TYPE_UTILITY));

end

function update()
end

function kill()

end