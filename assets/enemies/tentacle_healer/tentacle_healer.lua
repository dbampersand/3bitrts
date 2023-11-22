local splash = -1
local waterspout = -1

local timer = 0
local castAfter = 2


function setup()
    SetSprite("assets/enemies/tentacle_healer/tentacle.png");

    SetDamage(10);
    SetMaxHP(850,true)
    SetSpeed(0)
    SetRange(30);

    splash = AddAbility(GetObjRef(),"assets/enemies/tentacle_healer/ability_splash.lua",0);


    SetAttackSounds({
        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });

    SetAbilityCooldownTimer(GetObjRef(),splash,GetAbilityCooldown(GetObjRef(),splash));
end

function update(dt)
    if (IsInCombat()) then
        if (AbilityIsOnCooldown(GetObjRef(),splash) == false) then
            local objs = GetAllObjsByFriendliness(GetObjFriendliness(GetObjRef()))
            local lowestHP = 100
            local currentTarget = -1
            for i = 1, #objs do
                if (objs[i] ~= GetObjRef()) then
                    if (GetHPPercent(objs[i]) < lowestHP) then
                        currentTarget = objs[i]
                    end
                end
            end
            if (currentTarget >= 0) then
                CastAbility(splash,2,{{target = currentTarget}})
            end
        end
    end
end

function kill()

end