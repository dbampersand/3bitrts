local maxHP = 1000

function setup()
    SetSprite("assets/friendly/rune_smith/rune_smith.png");
    AddAbility(GetObjRef(),"assets/friendly/rune_smith/rune_of_lifesteal.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/rune_smith/rune_of_pain.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/rune_smith/rune_of_protection.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/rune_smith/rune_of_shield.lua",3)    
    AddAbility(GetObjRef(),"assets/friendly/rune_smith/rune_of_menace.lua",4)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(12);

    SetObjType(TYPE_TANK)
    SetThreatMultiplier(12);
    IsPlayerChoosable(true);
    SetCategory(TYPE_TANK);

    SetMaxHP(maxHP,true);

    SetObjPurchaseScreenSprite("assets/friendly/rune_smith/rune_smith_full.png")
    SetObjName("Rune Smith");
    SetObjDescription("Not simply satisfied with her fledgling smithy, the Rune Smith took forging weapons and armor to a new level, introducing a concept few had tried: powerful magic runes. Crafting these items, however, requires extremely rare materials, and all but scraps turn up through travelling sellers. A large vein of ore had been rumored for a long time inside the mountain, and the Rune Smith intends to find it.")
    SetObjCost(10);
end

function update(dt)
end

function kill()

end