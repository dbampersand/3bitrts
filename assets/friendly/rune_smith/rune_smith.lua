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

    SetMaxHP(1000,true);

    SetObjPurchaseScreenSprite("assets/friendly/rune_smith/rune_smith_full.png")
    SetObjName("Rune Smith");
    SetObjDescription("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ut quam efficitur, mattis lacus vitae, fermentum nisl. Suspendisse vel sodales mi. Pellentesque eleifend auctor nulla, at ornare quam. Cras ac lorem sed tortor porttitor rhoncus. Fusce sed arcu ut ex mattis interdum non et massa. Nunc venenatis justo eu mauris dapibus pretium. Etiam nec nisl ut nibh euismod accumsan sed nec ante. Aliquam molestie est vitae arcu tristique gravida. Nunc non sapien eu nulla commodo varius. Curabitur lectus quam, condimentum eget posuere sed, eleifend eget eros. Donec tincidunt dolor in velit dictum, nec convallis erat efficitur. Mauris lacinia suscipit elit at interdum. Proin leo turpis, mollis at sodales a, consequat sit amet massa. Proin tristique libero ut velit mollis aliquam. Nullam sodales velit quis augue pharetra, ut porta nulla pharetra. Pellentesque at ante accumsan, semper ex sit amet, imperdiet purus.")
    SetObjCost(10);
end

function update(dt)
end

function kill()

end