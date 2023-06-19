local targ = -1

local grasp = -1
local toll = -1

function setup()
    SetSprite("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.png");
    SetChannelingSprite("assets/enemies/skeleton_minion_melee/skeleton_minion_melee_channelling.png");

    SetDamage(15);
    SetMaxHP(500,true)
    SetSpeed(80)
    SetRange(50);

    toll = AddAbility(GetObjRef(),"assets/enemies/skeleton_minion_melee/ability_toll.lua",0);
    grasp = AddAbility(GetObjRef(),"assets/enemies/skeleton_minion_melee/ability_grasp.lua",1);

    SetAttackSounds(
    {
        "assets/audio/attacks/melee_dagger/dagger_1.wav",
        "assets/audio/attacks/melee_dagger/dagger_2.wav",
        "assets/audio/attacks/melee_dagger/dagger_3.wav",
        "assets/audio/attacks/melee_dagger/dagger_4.wav",
        "assets/audio/attacks/melee_dagger/dagger_5.wav"
    }
    );

end

function update(dt)
    if (IsInCombat()) then
        local attacking = GetAttackTarget(GetObjRef());
        if (IsInAttackRange(GetObjRef(),attacking)) then
            CastAbility(toll,0,{{}});
            CastAbility(grasp,0.5,{{target = GetAttackTarget(GetObjRef()) }} )
        end

    end
end

function kill()

end