local maxHP = 300;

function setup()
    SetSprite("assets/friendly/rogue/rogue.png");

    AddAbility(GetObjRef(),"assets/friendly/rogue/shiv.lua",0)
    AddAbility(GetObjRef(),"assets/friendly/rogue/cloak.lua",1)
    AddAbility(GetObjRef(),"assets/friendly/rogue/backstab.lua",2)
    AddAbility(GetObjRef(),"assets/friendly/rogue/toxic.lua",3)
    AddAbility(GetObjRef(),"assets/friendly/rogue/summon_cabal.lua",4)
    --AddAbility(GetObjRef(),"assets/friendly/rogue/dash.lua",4)


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(15);
    SetRange(1)
    SetSpeed(60)
    SetMaxHP(maxHP,true);

    SetObjType(TYPE_MELEEDPS);

    IsPlayerChoosable(true);    
    SetCategory(TYPE_MELEEDPS);
    
    SetObjectPush(false);

    SetObjPurchaseScreenSprite("assets/friendly/rogue/rogue_full.png")
    SetObjName("Rogue");
    SetObjDescription("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ut quam efficitur, mattis lacus vitae, fermentum nisl. Suspendisse vel sodales mi. Pellentesque eleifend auctor nulla, at ornare quam. Cras ac lorem sed tortor porttitor rhoncus. Fusce sed arcu ut ex mattis interdum non et massa. Nunc venenatis justo eu mauris dapibus pretium. Etiam nec nisl ut nibh euismod accumsan sed nec ante. Aliquam molestie est vitae arcu tristique gravida. Nunc non sapien eu nulla commodo varius. Curabitur lectus quam, condimentum eget posuere sed, eleifend eget eros. Donec tincidunt dolor in velit dictum, nec convallis erat efficitur. Mauris lacinia suscipit elit at interdum. Proin leo turpis, mollis at sodales a, consequat sit amet massa. Proin tristique libero ut velit mollis aliquam. Nullam sodales velit quis augue pharetra, ut porta nulla pharetra. Pellentesque at ante accumsan, semper ex sit amet, imperdiet purus.")

end

function update(dt)
end

function kill()

end