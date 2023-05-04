local bite = -1
local wave = -1

function setup()
    SetSprite("assets/enemies/giant_worm/giant_worm.png");

    SetDamage(20);
    SetMaxHP(1200,true)
    SetSpeed(80)
    SetRange(50);

    bite = AddAbility(GetObjRef(),"assets/enemies/giant_worm/ability_spin_web.lua",0);

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
        CastAbility(bite,2,{target = GetAttackTarget(GetObjRef())});
        
    end
end

function kill()
end