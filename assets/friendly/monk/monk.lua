local maxHP = 800;

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

    SetDamage(35);
    SetRange(5)
    SetSpeed(60)
    SetMaxHP(maxHP,true);

    SetObjType(TYPE_MELEEDPS);

    IsPlayerChoosable(true);
    SetCategory(TYPE_MELEEDPS);
    
    SetObjectPush(false);

    SetObjPurchaseScreenSprite("assets/friendly/monk/monk_full.png")
    SetObjName("Monk");
    SetObjDescription("His birthplace ravaged by civil war, the Monk was villainized by the conquering warlords, being scapegoated as the reason for the violence. Once a peaceful soul, he had to learn to fight to survive. As he left his home, he wandered the earth until meeting with a band of adventurers.")

    SetObjCost(100);
    

    SetAttackSounds(
        {
        "assets/audio/attacks/melee_hammer/hammer_1.wav",
        "assets/audio/attacks/melee_hammer/hammer_2.wav",
        "assets/audio/attacks/melee_hammer/hammer_3.wav",
        "assets/audio/attacks/melee_hammer/hammer_4.wav",
        "assets/audio/attacks/melee_hammer/hammer_5.wav",
        "assets/audio/attacks/melee_hammer/hammer_6.wav"
    }
    );

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

    
        --heal = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES,COLOR_DEFAULT,DITHER_EIGTH,false, -1, {f1})
    end
    if (heal > -1) then
        --MoveAttack(heal,GetX(GetObjRef()),GetY(GetObjRef()));
        --SetAttackLifetime(heal, 10)
    end

end

function kill()
    RemoveAttack(heal);
    heal = -1
end