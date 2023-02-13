function setup()
    SetSprite("assets/enemies/viper/viper.png");

    SetDamage(20);
    SetMaxHP(500,true)
    SetSpeed(80)
    SetRange(5);

    AddAbility(GetObjRef(),"assets/enemies/viper/ability_bite.lua",0);

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
    local threatlist = GetThreatRank();
    local target = {};
    target["target"] = threatlist[1];
    if (target["target"] ~= nil) then
        CastAbility(0,0,{target});
    end
end

function kill()

end