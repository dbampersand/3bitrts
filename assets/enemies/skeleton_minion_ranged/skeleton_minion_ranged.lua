
local rain = -1

function setup()
    SetSprite("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.png");

    SetDamage(35);
    SetMaxHP(350,true)
    SetSpeed(80)
    SetRange(140);

    rain = AddAbility(GetObjRef(),"assets/enemies/skeleton_minion_ranged/ability_rain.lua",0);

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
        CastAbility(rain,1,{{target = GetAttackTarget(GetObjRef()) }} )

    end
end

function kill()

end