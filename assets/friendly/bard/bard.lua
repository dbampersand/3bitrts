local maxHP = 250;


function setup()
    SetSprite("assets/friendly/bard/bard.png");
    AddAbility(GetObjRef(),"assets/friendly/bard/aoe_speed.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/bard/aoe_damage.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/bard/aoe_shield.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/bard/aoe_heal.lua",3)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(10);
    SetRange(30);
    SetMaxHP(maxHP,true);   


    SetObjType(Bor(TYPE_HEALER,TYPE_UTILITY));

    IsPlayerChoosable(true);
    SetCategory(TYPE_UTILITY);
    
    SetObjPurchaseScreenSprite("assets/friendly/bard/bard_full.png")
    SetObjName("Bard");
    SetObjDescription("")
    
end

function update(dt)
end

function kill()

end