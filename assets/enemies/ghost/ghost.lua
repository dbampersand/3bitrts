
local terror = -1
local timeBetweenCasts = 0.2

local casting = false
local timer = timeBetweenCasts

function setup()
    SetSprite("assets/enemies/ghost/ghost.png");
    SetChannelingSprite("assets/enemies/ghost/ghost_channelling.png");

    SetDamage(60);
    SetMaxHP(1200,true)
    SetSpeed(60)
    SetRange(80);

    terror = AddAbility(GetObjRef(),"assets/enemies/ghost/ability_terror.lua",0);

    SetAttackSounds({
        "assets/audio/attacks/attack_spit/spit_1.wav",
        "assets/audio/attacks/attack_spit/spit_2.wav",
        "assets/audio/attacks/attack_spit/spit_3.wav",
        "assets/audio/attacks/attack_spit/spit_4.wav",
        "assets/audio/attacks/attack_spit/spit_5.wav",

        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });

end

function update(dt)
    if (IsInCombat()) then
        if (GetAbilityStacks(GetObjRef(),terror) == GetMaxStacks(GetObjRef(),terror)) then
            casting = true
        end
        if (GetAbilityStacks(GetObjRef(),terror) == 0) then
            casting = false
        end
        if (casting) then
            timer = timer - dt
            if (timer < 0) then
                CastAbility(terror,0.2,{})
                timer = timeBetweenCasts
            end
        end
    end
end

function kill()

end