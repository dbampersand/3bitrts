
local timer = 0;
local castTimer = 0

local snare = -1
local spines = -1
function setup()
    SetSprite("assets/enemies/prowler/prowler.png");
    SetChannelingSprite("assets/enemies/prowler/prowler_channelling.png");

    SetDamage(25);
    SetMaxHP(3500,true)
    SetSpeed(0)
    SetRange(0);
    SetObjectPushable(GetObjRef(),false)
    spines = AddAbility(GetObjRef(),"assets/enemies/prowler/ability_spines.lua",0);
    snare = AddAbility(GetObjRef(),"assets/enemies/prowler/ability_snare.lua",1);

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

        timer = timer + dt;

        local objToAttack = GetRandomUnit(TYPE_ENEMY,TYPE_ANY,GetAbilityRange(GetObjRef(),spines))[1]

        local target = GetHeadingVector(GetX(GetObjRef()),GetY(GetObjRef()),GetX(objToAttack),GetY(objToAttack));
        target["target"]  = objToAttack;


        CastAbility(spines,0.95,{target});
    end

end

function kill()
end

