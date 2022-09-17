local maxHP = 250;

function setup()
    SetSprite("assets/friendly/ranger/ranger.png");

    AddAbility("assets/friendly/ranger/aimed_shot.lua",0)
    AddAbility("assets/friendly/ranger/cripple.lua",1)
    AddAbility("assets/friendly/ranger/flurry.lua",2)
    AddAbility("assets/friendly/ranger/instinct.lua",3)
    AddAbility("assets/friendly/ranger/galeshot.lua",4)


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(15);
    SetRange(30)
    SetSpeed(60)
    SetMaxHP(maxHP,true);

    SetObjType(TYPE_RANGEDDPS);

    IsPlayerChoosable(true);
    SetCategory(TYPE_RANGEDDPS);
    
    SetObjectPush(false);
end

function update(dt)
end

function kill()

end