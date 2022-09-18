local maxHP = 250;

local heal = -1

function setup()
    SetSprite("assets/friendly/monk/monk.png");

    AddAbility("assets/friendly/monk/palm.lua",0)
    AddAbility("assets/friendly/monk/kick.lua",1)
    AddAbility("assets/friendly/monk/fury.lua",2)
    AddAbility("assets/friendly/monk/whirlwind.lua",3)
    AddAbility("assets/friendly/monk/lotus.lua",4)


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