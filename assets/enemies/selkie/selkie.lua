local targ = -1

local transform = -1
local tide_pool = -1
function setup()
    SetSprite("assets/enemies/selkie/selkie.png");

    SetDamage(10);
    SetMaxHP(500,true)
    SetSpeed(120)
    SetRange(5);

    transform = AddAbility(GetObjRef(),"assets/enemies/selkie/ability_transform.lua",0);
    tide_pool = AddAbility(GetObjRef(),"assets/enemies/selkie/ability_tide_pool.lua",1);

    local targ = GetRandomUnit(TYPE_ENEMY,Bor(TYPE_HEALER,TYPE_RANGEDDPS),999)[1]

    SetAttackSounds({
        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });

end

function update(dt)
    if (GetHPPercent(GetObjRef()) < 50) then
        CastAbility(transform,0,{})
        CastAbility(tide_pool,1,{})
    end
end

function kill()

end