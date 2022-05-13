function setup()
    SetSprite("assets/friendly/warrior/warrior.png");
    AddAbility("assets/friendly/warrior/strike.lua",0)    
    AddAbility("assets/friendly/warrior/shield.lua",1)    
    AddAbility("assets/friendly/warrior/taunt.lua",2)    
    AddAbility("assets/friendly/warrior/charge.lua",3)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(5);

    SetObjType(TYPE_TANK)
    SetThreatMultiplier(5);
    IsPlayerChoosable(true);


end

function update()
end

function kill()

end