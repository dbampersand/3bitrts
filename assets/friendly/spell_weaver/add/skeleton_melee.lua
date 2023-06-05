local maxHP = 750;


function setup()
    SetSprite("assets/friendly/necromancer/add/skeleton_melee.png");
    AddAbility(GetObjRef(),"assets/friendly/necromancer/add/bolt.lua",0)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(10);
    SetRange(15);
    SetMaxHP(maxHP,true);   


    SetObjType(Bor(TYPE_UTILITY));
    
    SetObjPurchaseScreenSprite("assets/friendly/necromacner/necromancer_full.png")
    SetObjName("Bow Skeleton");
    SetLifetime(GetObjRef(),30)
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