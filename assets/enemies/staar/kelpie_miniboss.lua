local charge = -1
local waterspout = -1

local timer = 0
local castAfter = 2


function setup()
    SetSprite("assets/enemies/kelpie_miniboss/kelpie_miniboss.png");

    SetDamage(40);
    SetMaxHP(1200,true)
    SetSpeed(120)
    SetRange(5);

    charge = AddAbility(GetObjRef(),"assets/enemies/kelpie_miniboss/ability_charge.lua",0);
    waterspout = AddAbility(GetObjRef(),"assets/enemies/kelpie_miniboss/ability_waterspout.lua",1);


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
            CastAbility(charge,1.5,{{target = GetRandomUnit(TYPE_ENEMY,Bor(Bor(TYPE_HEALER,TYPE_RANGEDDPS),TYPE_MELEEDPS),999)[1]}})
            CastAbility(waterspout,1,{{target = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,256)[1]}})
        end
    end
end

function kill()

end