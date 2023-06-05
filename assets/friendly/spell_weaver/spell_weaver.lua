local maxHP = 750;


function setup()
    SetSprite("assets/friendly/spell_weaver/spell_weaver.png");
    AddAbility(GetObjRef(),"assets/friendly/spell_weaver/stitch.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/spell_weaver/counterbalance.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/spell_weaver/cocoon.lua",2)    
     AddAbility(GetObjRef(),"assets/friendly/spell_weaver/weave.lua",3)    

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(10);
    SetRange(30);
    SetMaxHP(maxHP,true);   


    SetObjType(TYPE_UTILITY);

    IsPlayerChoosable(true);
    SetCategory(TYPE_UTILITY);
    
    SetObjPurchaseScreenSprite("assets/friendly/spell_weaver/spell_weaver_full.png")
    SetObjName("Spell Weaver");
    SetObjDescription("A farmer slave, killed in a machine accident, thought his labour was finally over. His rest was disturbed by a passing wizard, promising a subservient zombie, more than one lifetime of labour for a small fee. But the foolhardy wizard, young and untrained, focused too much energy into the ressurection spell, and ended up creating a monster: a true necromancer. Fueled by cruel memories of his past life, he despises mankind, and works only to return nature to its intended path.")
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