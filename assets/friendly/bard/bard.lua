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
    SetObjDescription("A travelling musician. Finding the power of their songs at a young age, they found an income hard to come by: most brushed off the sell of a magic song as hogwash, or worse, witchcraft. The only people who would accept them and their unique talent, the life of an adventurer called. Lute in hand, they sing powerful magic songs providing their allies vital utility.")
    
end

function update(dt)
    if (GetKey(KEY_U)) then
    CreateObject("assets/enemies/viper/viper.lua",GetMouseWorldX(),GetMouseWorldY(),TYPE_ENEMY,3); 
    end
end

function kill()

end