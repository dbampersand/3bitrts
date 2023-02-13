local maxHP = 100

function setup()
    SetSprite("assets/enemies/wyrm_boss/wyrm_add/smallsnake.png");
    AddAbility(GetObjRef(),"assets/enemies/wyrm_boss/ability_bite.lua",0)   
    SetDamage(20);
    SetMaxHP(maxHP,true)
    SetSpeed(10)

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
end

function kill()

end