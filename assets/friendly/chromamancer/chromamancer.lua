local maxHP = 450;
local maxMana = 150

function setup()
    SetSprite("assets/friendly/chromamancer/chronomancer.png");
    
    AddAbility(GetObjRef(),"assets/friendly/chromamancer/illusion.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/chromamancer/chromophore.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/chromamancer/iridesce.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/chromamancer/saturate.lua",3)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(5);
    SetRange(40)

    SetManaRegen(15);
    SetMaxMana(maxMana,true)
    SetMaxHP(maxHP,true);
    
    SetObjType(TYPE_HEALER)

    IsPlayerChoosable(true);
    SetCategory(TYPE_HEALER);
    SetObjectPush(false);
    ObjectUsesMana(GetObjRef(),true);

    SetObjPurchaseScreenSprite("assets/friendly/chromamancer/chromamancer_full.png")
    SetObjName("Chromamancer");
    SetObjDescription("At a young age, the Chromamancer found they could bend light at will. Where they went, fire followed: uncontrolled, the bending of light tended to focus like a spyglass in the sun, leading the Chromamancer to exile. With a dazzling array of illusory tricks at their command, the Chromamancer swore an oath to use their powers to shield their allies from harm.")


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