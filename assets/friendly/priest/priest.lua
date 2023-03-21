local maxHP = 800;

function setup()
    SetSprite("assets/friendly/priest/priest.png");
    AddAbility(GetObjRef(),"assets/friendly/priest/smite.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/priest/circleofhealing.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/priest/regen.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/priest/shield.lua",3)    
    AddAbility(GetObjRef(),"assets/friendly/priest/greaterheal.lua",4)    


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(5);
    SetRange(100)
    SetMaxHP(maxHP,true)
    SetManaRegen(0.85);

    SetObjType(TYPE_HEALER);
    IsPlayerChoosable(true);

    SetCategory(TYPE_HEALER);

    ObjectUsesMana(GetObjRef(),true);

    SetObjPurchaseScreenSprite("assets/friendly/priest/priest_full.png")
    SetObjName("Priest");
    SetObjDescription("To a member of the ancient and exclusive Triadi church, three represent the holiness above God. This priestess belongs to a small sect of the church believing that God can be found inside of a mountain. That prospect alone made her join a party of adventurers tackling the mountain as a healer.")
   
    SetAttackSounds(
        {
        "assets/audio/attacks/ranged_magic/magic_1.wav",
        "assets/audio/attacks/ranged_magic/magic_2.wav",
        "assets/audio/attacks/ranged_magic/magic_3.wav",
        "assets/audio/attacks/ranged_magic/magic_4.wav",
        "assets/audio/attacks/ranged_magic/magic_5.wav",
        "assets/audio/attacks/ranged_magic/magic_6.wav"
    }
    );


end

function update(dt)
end

function kill()

end