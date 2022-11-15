function setup()
    SetSprite("assets/friendly/paladin/paladin.png");
    AddAbility(GetObjRef(),"assets/friendly/paladin/shield_slam.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/paladin/holy_word.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/paladin/circle_of_protection.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/paladin/lay_on_hands.lua",3)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(12);

    SetObjType(TYPE_TANK)
    SetThreatMultiplier(12);
    IsPlayerChoosable(true);
    SetCategory(TYPE_TANK);

    SetMaxHP(1000,true);
    
    ObjectUsesMana(GetObjRef(),true);

    SetObjPurchaseScreenSprite("assets/friendly/paladin/paladin_full.png")
    SetObjName("Paladin");
    SetObjDescription("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ut quam efficitur, mattis lacus vitae, fermentum nisl. Suspendisse vel sodales mi. Pellentesque eleifend auctor nulla, at ornare quam. Cras ac lorem sed tortor porttitor rhoncus. Fusce sed arcu ut ex mattis interdum non et massa. Nunc venenatis justo eu mauris dapibus pretium. Etiam nec nisl ut nibh euismod accumsan sed nec ante. Aliquam molestie est vitae arcu tristique gravida. Nunc non sapien eu nulla commodo varius. Curabitur lectus quam, condimentum eget posuere sed, eleifend eget eros. Donec tincidunt dolor in velit dictum, nec convallis erat efficitur. Mauris lacinia suscipit elit at interdum. Proin leo turpis, mollis at sodales a, consequat sit amet massa. Proin tristique libero ut velit mollis aliquam. Nullam sodales velit quis augue pharetra, ut porta nulla pharetra. Pellentesque at ante accumsan, semper ex sit amet, imperdiet purus.")
    SetObjCost(20);
end

function update(dt)
end

function kill()

end