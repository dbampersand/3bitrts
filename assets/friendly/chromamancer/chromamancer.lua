local maxHP = 350;
local maxMana = 150

function setup()
    SetSprite("assets/friendly/chromamancer/chronomancer.png");
    
    AddAbility("assets/friendly/chromamancer/illusion.lua",0)    
    AddAbility("assets/friendly/chromamancer/chromophore.lua",1)    
    AddAbility("assets/friendly/chromamancer/iridesce.lua",2)    
    AddAbility("assets/friendly/chromamancer/saturate.lua",3)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(5);
    SetRange(40)

    SetManaRegen(15);
    SetMaxMana(maxMana,true)
    SetMaxHP(maxHP,true);
    
    SetObjType(TYPE_HEALER)
    SetThreatMultiplier(1);

    IsPlayerChoosable(true);
    SetCategory(TYPE_HEALER);
    SetObjectPush(false);
end

function update(dt)
end

function kill()

end