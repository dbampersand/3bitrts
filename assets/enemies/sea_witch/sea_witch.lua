local dark_tide = -1
local summon_tentacle = -1

local timer = 0
local castAfter = 2


function setup()
    SetSprite("assets/enemies/kelpie_miniboss/kelpie_miniboss.png");

    SetDamage(40);
    SetMaxHP(1200,true)
    SetSpeed(120)
    SetRange(5);

    dark_tide = AddAbility(GetObjRef(),"assets/enemies/sea_witch/ability_dark_tide.lua",0);
    summon_tentacle = AddAbility(GetObjRef(),"assets/enemies/sea_witch/ability_summon_tentacle.lua",1);


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
        if (ObjIsChannelling(GetObjRef()) == false) then
            timer = timer + dt
        end
        if (timer > castAfter) then
            timer = 0
            --CastAbility(charge,1.5,{{target = GetRandomUnit(TYPE_ENEMY,Bor(Bor(TYPE_HEALER,TYPE_RANGEDDPS),TYPE_MELEEDPS),999)[1]}})
            --CastAbility(waterspout,1,{{target = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,256)[1]}})
        end
    end
end

function kill()

end