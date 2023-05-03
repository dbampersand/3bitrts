local targ = -1
local attacking = false

local timer = 0
local timeBetweenShots = 0.1
local spit = -1
function setup()
    SetSprite("assets/enemies/spitter/spitter.png");

    SetDamage(20);
    SetMaxHP(900,true)
    SetSpeed(80)
    SetRange(50);

    spit = AddAbility(GetObjRef(),"assets/enemies/spitter/ability_spit.lua",0);

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
        if (IsAlive(targ) == false or GetDist(GetObjRef(),targ) > GetAbilityRange(GetObjRef(),spit) or GetStacks(GetObjRef(),spit) == 0) then
            targ = GetRandomUnit(TYPE_ENEMY,TYPE_ANY,999,1)[1];
        end    
        if (GetStacks(GetObjRef(),spit) == GetMaxStacks(GetObjRef(),spit)) then
            attacking = true;
        end
        if (GetStacks(GetObjRef(),spit) == 0) then
            attacking = false; 
        end
        if (attacking) then
            timer = timer - dt
            if (timer <= 0) then
                timer = timeBetweenShots
                CastAbility(spit,0,{{target = targ}});
            end
        end
    end
end

function kill()

end