local wnip = -1
local vision = -1

local timer = 0
local castAfter = 2


function setup()
    SetSprite("assets/enemies/staar/staar.png");

    SetDamage(60);
    SetMaxHP(3000,true)
    SetSpeed(120)
    SetRange(80);

    whip = AddAbility(GetObjRef(),"assets/enemies/staar/ability_whip.lua",0);


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
    end
end

function kill()

end