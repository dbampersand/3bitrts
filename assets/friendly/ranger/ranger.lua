local maxHP = 800;

function setup()
    SetSprite("assets/friendly/ranger/ranger.png");

    AddAbility(GetObjRef(),"assets/friendly/ranger/aimed_shot.lua",0)
    AddAbility(GetObjRef(),"assets/friendly/ranger/cripple.lua",1)
    AddAbility(GetObjRef(),"assets/friendly/ranger/flurry.lua",2)
    AddAbility(GetObjRef(),"assets/friendly/ranger/instinct.lua",3)
    AddAbility(GetObjRef(),"assets/friendly/ranger/galeshot.lua",4)


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(25);
    SetRange(75)
    SetSpeed(60)
    SetMaxHP(maxHP,true);

    SetObjType(TYPE_RANGEDDPS);

    IsPlayerChoosable(true);
    SetCategory(TYPE_RANGEDDPS);
    
    SetObjectPush(false);

    SetObjPurchaseScreenSprite("assets/friendly/ranger/ranger_full.png")
    SetObjName("Ranger");
    SetObjDescription("Hailing from a treetop city, the mountains are not a natural place to find the Ranger, but her desire for excitement in the idyllic forest was too much. The sharpshooter, leaving her home, quickly realised how big the world is. Destitute after being taken by a scam, she stumbled upon work with a party of adventurers.")

    SetObjCost(0);

    SetAttackSounds(
        {
        "assets/audio/attacks/ranged_bow/bow_1.wav",
        "assets/audio/attacks/ranged_bow/bow_2.wav",
        "assets/audio/attacks/ranged_bow/bow_3.wav",
        "assets/audio/attacks/ranged_bow/bow_4.wav",
        "assets/audio/attacks/ranged_bow/bow_5.wav",
        "assets/audio/attacks/ranged_bow/bow_6.wav",
        "assets/audio/attacks/ranged_bow/bow_7.wav",
        "assets/audio/attacks/ranged_bow/bow_8.wav"
    }
    );

end

function update(dt)
end

function kill()

end