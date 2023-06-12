local targ = -1
local attacking = false

local bash = -1

function setup()
    SetSprite("assets/enemies/skeleton_basher/skeleton_basher.png");
    SetChannelingSprite("assets/enemies/skeleton_basher/skeleton_basher_channelling.png");

    SetDamage(20);
    SetMaxHP(900,true)
    SetSpeed(80)
    SetRange(50);

    bash = AddAbility(GetObjRef(),"assets/enemies/skeleton_basher/ability_bash.lua",0);

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
        CastAbility(bash,0.2,{{target = GetAttackTarget(GetObjRef()) }});
    end
end

function kill()

end