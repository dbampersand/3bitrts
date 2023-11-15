local whip = -1
local spike = -1

local timer = 0
local castAfter = 1.25


function setup()
    SetSprite("assets/enemies/staar/staar.png");

    SetDamage(120);
    SetMaxHP(5500,true)
    SetSpeed(120)
    SetRange(80);

    whip = AddAbility(GetObjRef(),"assets/enemies/staar/ability_whip.lua",0);
    spike = AddAbility(GetObjRef(),"assets/enemies/staar/ability_spike.lua",1);



    SetAttackSounds({
        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });

end

function update(dt)
    if (IsInCombat()) then
        timer = timer + dt
        if (timer > castAfter) then
            timer = 0
            CastAbility(whip,1.5,{})
        end
        if (AbilityIsOnCooldown(whip)) then
            CastAbility(spike,0.4,{})
        end
    end
end

function kill()

end