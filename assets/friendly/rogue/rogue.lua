local maxHP = 300;

function setup()
    SetSprite("assets/friendly/rogue/rogue.png");
    AddAbility("assets/friendly/rogue/shiv.lua",0)    
    AddAbility("assets/friendly/rogue/dash.lua",1)    
    AddAbility("assets/friendly/rogue/backstab.lua",2)    
    AddAbility("assets/friendly/rogue/cloak.lua",3)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(15);
    SetRange(1)
    SetMaxHP(maxHP,true);

    SetObjType(TYPE_MELEEDPS);

    IsPlayerChoosable(true);
    SetCategory(TYPE_MELEEDPS);

end

function update(dt)
end

function kill()

end