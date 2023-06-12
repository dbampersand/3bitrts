local raise = -1
local frostBlast = -1


function setup()
    SetSprite("assets/enemies/apprentice_lich/apprentice_lich.png");
    SetChannelingSprite("assets/enemies/apprentice_lich/apprentice_lich_casting_frost_blast.png");
    
    SetDamage(20);
    SetMaxHP(900,true)
    SetSpeed(80)
    SetRange(50);

    raise = AddAbility(GetObjRef(),"assets/enemies/apprentice_lich/ability_raise.lua",0);
    frostBlast = AddAbility(GetObjRef(),"assets/enemies/apprentice_lich/ability_frost_blast.lua",1);
    


    SetAttackSounds({
        "assets/audio/attacks/attack_spit/spit_1.wav",
        "assets/audio/attacks/attack_spit/spit_2.wav",
        "assets/audio/attacks/attack_spit/spit_3.wav",
        "assets/audio/attacks/attack_spit/spit_4.wav",
        "assets/audio/attacks/attack_spit/spit_5.wav",

        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });

end

function update(dt)
    if (IsInCombat()) then
        CastAbility(raise,0,{{}});
        CastAbility(frostBlast,1,{{target = GetAttackTarget(GetObjRef()) }});


    end
end

function kill()

end