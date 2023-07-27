local dark_shot = -1
local waterspout = -1

local timer = 0
local castAfter = 2


function setup()
    SetSprite("assets/enemies/tentacle/tentacle.png");

    SetDamage(10);
    SetMaxHP(1200,true)
    SetSpeed(0)
    SetRange(30);

    dark_shot = AddAbility(GetObjRef(),"assets/enemies/tentacle/ability_dark_shot.lua",0);
    thrash = AddAbility(GetObjRef(),"assets/enemies/tentacle/ability_thrash.lua",1);


    SetAttackSounds({
        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });
    SetAbilityCooldownTimer(GetObjRef(),thrash,GetAbilityCooldown(GetObjRef(),thrash))

end

function update(dt)
    if (IsInCombat() == false) then
        SetAbilityCooldownTimer(GetObjRef(),thrash,GetAbilityCooldown(GetObjRef(),thrash))
    end
    if (IsInCombat()) then
        CastAbility(dark_shot,1,{{target = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,GetAbilityRange(GetObjRef(),dark_shot))[1]}})
        CastAbility(thrash,1,{})
    end
end

function kill()

end