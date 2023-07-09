local sea_shell = -1
local spawn_tentacle = -1

local timer = 0
local castAfter = 2


function setup()
    SetSprite("assets/enemies/sea_witch/sea_witch.png");

    SetDamage(40);
    SetMaxHP(2400,true)
    SetSpeed(120)
    SetRange(5);

    sea_shell = AddAbility(GetObjRef(),"assets/enemies/sea_witch/ability_sea_shell.lua",0);
    ability_spawn_tentacle = AddAbility(GetObjRef(),"assets/enemies/sea_witch/ability_spawn_tentacle.lua",1);


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
        CastAbility(sea_shell,1,{{target = GetAttackTarget(GetObjRef())}})
        CastAbility(ability_spawn_tentacle,1,{})
        
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