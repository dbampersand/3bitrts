local maxHP = 1100

function setup()
    SetSprite("assets/friendly/warrior/warrior.png");
    AddAbility(GetObjRef(),"assets/friendly/warrior/strike.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/warrior/shield.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/warrior/taunt.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/warrior/charge.lua",3)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(5);

    SetObjType(TYPE_TANK)
    SetThreatMultiplier(10);
    IsPlayerChoosable(true);
    SetCategory(TYPE_TANK);

    SetMaxHP(1100,true);

    SetObjPurchaseScreenSprite("assets/friendly/warrior/warrior_full.png")
    SetObjName("Warrior");
    SetObjDescription("A hired bodyguard, the Warrior personifies a strong and no-frills style. With sword and shield, he defends his allies with a mighty presence, having trained under the royal guard. Well known for his cautious and well-planned approach, he embodies the qualities of a leader.")

    SetAttackSounds(
        {"assets/audio/attacks/melee_sword/sword_1.wav",
        "assets/audio/attacks/melee_sword/sword_2.wav",
        "assets/audio/attacks/melee_sword/sword_3.wav",
        "assets/audio/attacks/melee_sword/sword_4.wav",
        "assets/audio/attacks/melee_sword/sword_5.wav",
        "assets/audio/attacks/melee_sword/sword_6.wav",
        "assets/audio/attacks/melee_sword/sword_7.wav",
    });
end 

function update(dt)
end

function kill()

end