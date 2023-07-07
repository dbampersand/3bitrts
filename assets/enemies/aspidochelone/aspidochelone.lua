local spin = -1
local tongue = -1
local spines = -1


local timer = 0
local castAfter = 5


function setup()
    SetSprite("assets/enemies/aspidochelone/aspidochelone.png");

    SetDamage(120);
    SetMaxHP(1200,true)
    SetSpeed(120)
    SetRange(5);

    spin = AddAbility(GetObjRef(),"assets/enemies/aspidochelone/ability_spin.lua",0);
    tongue = AddAbility(GetObjRef(),"assets/enemies/aspidochelone/ability_tongue.lua",1);
    spines = AddAbility(GetObjRef(),"assets/enemies/aspidochelone/ability_spines.lua",2);


    SetObjectPushable(GetObjRef(),false)

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
        CastAbility(spin,0,{})
        CastAbility(tongue,0,{})
        CastAbility(spines,2,{})


    end
end

function kill()

end