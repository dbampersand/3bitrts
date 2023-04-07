local bind = 0
local sinkholes = 0
local fangs = 0
function setup()
    SetSprite("assets/enemies/naja/naja.png");

    SetDamage(20);
    SetMaxHP(2000,true)
    SetSpeed(80)
    SetRange(50);

    bind = AddAbility(GetObjRef(),"assets/enemies/naja/ability_bind.lua",0);
    sinkholes = AddAbility(GetObjRef(),"assets/enemies/naja/ability_sinkholes.lua",1);
    fangs = AddAbility(GetObjRef(),"assets/enemies/naja/ability_fangs.lua",2);

    SetChannelingSprite("assets/enemies/naja/naja_channeling.png")

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

        CastAbility(fangs,0,{{target=GetRandomUnit(TYPE_ENEMY,TYPE_TANK,999)[1]}});

        CastAbility(sinkholes,0,{});
        local target = {};

        local targ = GetRandomUnit(TYPE_ENEMY,TYPE_ANY,999)[1];


        target["target"] = targ;
        if (target["target"] ~= nil) then
            CastAbility(bind,2,{target});
        end
    end
end

function kill()

end