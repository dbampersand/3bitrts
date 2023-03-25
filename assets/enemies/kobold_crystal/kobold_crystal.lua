local crash = 0

function setup()
    SetSprite("assets/enemies/kobold_crystal/kobold_crystal.png");

    SetDamage(0);
    SetMaxHP(750,true)
    SetSpeed(0)
    SetRange(32);
    SetInvincible(GetObjRef(),true)
    RemoveFromCount(GetObjRef(),true);

    crash = AddAbility(GetObjRef(),"assets/enemies/kobold_crystal/ability_crash.lua",0);


    SetAttackSounds({
        "assets/audio/attacks/melee_hammer/hammer_1.wav",
        "assets/audio/attacks/melee_hammer/hammer_2.wav",
        "assets/audio/attacks/melee_hammer/hammer_3.wav",
        "assets/audio/attacks/melee_hammer/hammer_4.wav",
        "assets/audio/attacks/melee_hammer/hammer_5.wav",
        "assets/audio/attacks/melee_hammer/hammer_6.wav"
    });

end

function update(dt)
    --if (AnyObjInCombat()) then
        CastAbility(crash,6,{{target=GetRandomUnit(TYPE_ENEMY,Band(Bnot(TYPE_TANK),TYPE_ALL),256)}});
    --end
end

function kill()

end