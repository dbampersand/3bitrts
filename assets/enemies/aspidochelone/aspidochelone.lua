local spin = -1
local tongue = -1
local spines = -1


local timer = 0
local castAfter = 5

local enraged = false


function setup()
    SetSprite("assets/enemies/aspidochelone/aspidochelone.png");

    SetDamage(65);
    SetMaxHP(4800,true)
    SetSpeed(40)
    SetRange(15);

    spin = AddAbility(GetObjRef(),"assets/enemies/aspidochelone/ability_spin.lua",0);
    tongue = AddAbility(GetObjRef(),"assets/enemies/aspidochelone/ability_tongue.lua",1);
    spines = AddAbility(GetObjRef(),"assets/enemies/aspidochelone/ability_spines.lua",2);
    summon = AddAbility(GetObjRef(),"assets/enemies/aspidochelone/ability_summon.lua",3);


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

        local target = GetAttackTarget(GetObjRef());
        local xT = GetX(target);
        local yT = GetY(target);

        CastAbility(summon,1,{x = xT, y = yT});

        if (GetHPPercent() <= 50 and enraged == false) then
            SetEnraged(GetObjRef(),true);
            enraged = true;
            
            local f1 = {};
            f1["trigger"] = TRIGGER_CONST;
            f1["type"] = EFFECT_COOLDOWN_RATE;
            f1["value"] = -0.3;
            f1["duration"] = 99999
            f1["stacks"] = false
        
            ApplyEffect(GetObjRef(),{f1})
        
        end


    end
end

function kill()

end