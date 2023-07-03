local maxHP = 7000

local breatheOut = -1
local wave = -1

local STATE_DEFAULT = 0
local STATE_WAVE = 0

local stateTimer = 0 


local state = STATE_DEFAULT

function setup()
    SetSprite("assets/enemies/stoor_worm/stoor_worm.png");
    --SetChannelingSprite("assets/enemies/wyrm_boss/wyrm_channeling.png");

    SetDamage(80);
    SetRange(256)
    SetMaxHP(maxHP,true)
    SetSpeed(0)
    SetObjectPush(true);
    ObjIsStunnable(GetObjRef(),false);
    SetObjIsBoss(GetObjRef(),true);

    breatheOut = AddAbility(GetObjRef(),"assets/enemies/stoor_worm/ability_breathe_out.lua",0)
    wave = AddAbility(GetObjRef(),"assets/enemies/stoor_worm/ability_wave.lua",1)

    SetAttackSounds({
        "assets/audio/attacks/attack_spit/spit_1.wav",
        "assets/audio/attacks/attack_spit/spit_2.wav",

        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });

end

function update(dt)
    if (IsInCombat()) then
        if (state == STATE_DEFAULT) then
            CastAbility(wave,1,{})
            
        end

    end
end

function kill()

end