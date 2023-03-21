local maxHP = 800;

function setup()
    SetSprite("assets/friendly/shaman/shaman.png");
    AddAbility(GetObjRef(),"assets/friendly/shaman/stoneskin_totem.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/shaman/swift_totem.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/shaman/flame_totem.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/shaman/healing_stream.lua",3)    
    AddAbility(GetObjRef(),"assets/friendly/shaman/healing_rain.lua",4)    


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(5);
    SetRange(100)
    SetMaxHP(maxHP,true)
    SetManaRegen(1);

    SetObjType(TYPE_HEALER);
    IsPlayerChoosable(true);

    SetCategory(TYPE_HEALER);

    ObjectUsesMana(GetObjRef(),true);

    SetObjPurchaseScreenSprite("assets/friendly/shaman/shaman_full.png")
    SetObjName("Shaman");
    SetObjDescription("Worshipping a long-buried and forgotten god, the Shaman still believes he hasn't been forsaken. Searching for a way to ressurect his fallen god, he travels the world extracting what knowledge locals know of magicks powerful enough to raise a god.")

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