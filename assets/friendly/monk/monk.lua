local maxHP = 250;

local heal = -1

function setup()
    SetSprite("assets/friendly/monk/monk.png");

    AddAbility(GetObjRef(),"assets/friendly/monk/palm.lua",0)
    AddAbility(GetObjRef(),"assets/friendly/monk/kick.lua",1)
    AddAbility(GetObjRef(),"assets/friendly/monk/fury.lua",2)
    AddAbility(GetObjRef(),"assets/friendly/monk/whirlwind.lua",3)
    AddAbility(GetObjRef(),"assets/friendly/monk/lotus.lua",4)


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05); 
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(15);
    SetRange(5)
    SetSpeed(60)
    SetMaxHP(maxHP,true);

    SetObjType(TYPE_MELEEDPS);

    IsPlayerChoosable(true);
    SetCategory(TYPE_MELEEDPS);
    
    SetObjectPush(false);

    SetObjPurchaseScreenSprite("assets/friendly/monk/monk_full.png")
    SetObjName("Monk");
    SetObjDescription("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ut quam efficitur, mattis lacus vitae, fermentum nisl. Suspendisse vel sodales mi. Pellentesque eleifend auctor nulla, at ornare quam. Cras ac lorem sed tortor porttitor rhoncus. Fusce sed arcu ut ex mattis interdum non et massa. Nunc venenatis justo eu mauris dapibus pretium. Etiam nec nisl ut nibh euismod accumsan sed nec ante. Aliquam molestie est vitae arcu tristique gravida. Nunc non sapien eu nulla commodo varius. Curabitur lectus quam, condimentum eget posuere sed, eleifend eget eros. Donec tincidunt dolor in velit dictum, nec convallis erat efficitur. Mauris lacinia suscipit elit at interdum. Proin leo turpis, mollis at sodales a, consequat sit amet massa. Proin tristique libero ut velit mollis aliquam. Nullam sodales velit quis augue pharetra, ut porta nulla pharetra. Pellentesque at ante accumsan, semper ex sit amet, imperdiet purus.")

end

function OnMapChange()
    heal = -1
end
function update(dt)
    if (heal == -1) then

        local f1 = {}
        f1["name"] = "Monk Heal"
        f1["trigger"] = TRIGGER_INSTANT
        f1["type"] = EFFECT_HEAL
        f1["value"] = 5;

    
        heal = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES,COLOR_DEFAULT,DITHER_EIGTH,false, -1, {f1})
    end
    if (heal > -1) then
        MoveAttack(heal,GetX(GetObjRef()),GetY(GetObjRef()));
        SetAttackLifetime(heal, 10)
    end

end

function kill()
    RemoveAttack(heal);
    heal = -1
end