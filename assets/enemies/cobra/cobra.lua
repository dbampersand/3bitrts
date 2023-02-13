    function setup()
    SetSprite("assets/enemies/viper/viper.png");

    SetDamage(20);
    SetMaxHP(650,true)
    SetSpeed(80)

    AddAbility(GetObjRef(),"assets/enemies/cobra/ability_spit.lua",0);
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
local timer = 0
function update(dt)
    timer = timer + dt;
    if (timer > 1) then
        timer = 0
        if (RandRange(0,5) <= 4) then
            do return end;
        end
    end
    local threatlist = GetThreatRank();
    local target = {};
    target["target"] = threatlist[1];
    if (target["target"] ~= nil) then
        CastAbility(0,4,{target});
    end
end

function kill()

end