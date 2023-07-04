local maxHP = 7000

local breatheOut = -1
local wave = -1
local part_waves = -1
local tsunami = -1
local whirlpool = -1

local STATE_DEFAULT = 0
local STATE_WAVE = 1

local stateTimer = 0 

local waveStateCooldown = 30
local waveStateLastsFor = 38


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
    part_waves = AddAbility(GetObjRef(),"assets/enemies/stoor_worm/ability_part_waves.lua",2)
    tsunami = AddAbility(GetObjRef(),"assets/enemies/stoor_worm/ability_tsunami.lua",3)
    whirlpool = AddAbility(GetObjRef(),"assets/enemies/stoor_worm/ability_whirlpool.lua",4)
    
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
            stateTimer = stateTimer + dt
            if (stateTimer > waveStateCooldown) then
                state = STATE_WAVE
                stateTimer = 0
            end
            CastAbility(breatheOut,1,{})
            CastAbility(whirlpool,1,{})
            CastAbility(tsunami,5,{})

        end
        if (state == STATE_WAVE) then
            stateTimer = stateTimer + dt
            if (stateTimer > waveStateLastsFor) then
                state = STATE_DEFAULT
                stateTimer = 0
                ClearChanneledAbility(GetObjRef())
                CastAbility(part_waves,1,{})
                SetAbilityCooldownTimer(breatheOut,4)
            end
            CastAbility(wave,1,{})
        end

    end
end

function kill()

end