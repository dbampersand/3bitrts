local maxHP = 1350

function setup()
    SetSprite("assets/friendly/paladin/paladin.png");
    AddAbility(GetObjRef(),"assets/friendly/paladin/shield_slam.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/paladin/holy_word.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/paladin/circle_of_protection.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/paladin/lay_on_hands.lua",3)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(12);

    SetObjType(TYPE_TANK)
    SetThreatMultiplier(12);
    IsPlayerChoosable(true);
    SetCategory(TYPE_TANK);

    SetMaxHP(maxHP,true);
    
    ObjectUsesMana(GetObjRef(),true);

    SetObjPurchaseScreenSprite("assets/friendly/paladin/paladin_full.png")
    SetObjName("Paladin");
    SetObjDescription("Blessed with holy power, the Paladin is a crusader for all things just. Confident that great evil lurks inside the mountain, she understands that clearing the mountain will be a test of her strength, will and faith. An ardent protector, the Paladin is committed to defending her allies above all else.")
    
    SetObjCost(800);

    SetAttackSounds(
        {
        "assets/audio/attacks/melee_sword/sword_1.wav",
        "assets/audio/attacks/melee_sword/sword_2.wav",
        "assets/audio/attacks/melee_sword/sword_3.wav",
        "assets/audio/attacks/melee_sword/sword_4.wav",
        "assets/audio/attacks/melee_sword/sword_5.wav",
        "assets/audio/attacks/melee_sword/sword_6.wav",
        "assets/audio/attacks/melee_sword/sword_7.wav"
    }
    );

end

function update(dt)
end

function kill()

end