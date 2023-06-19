local crash = 0

function setup()
    SetSprite("assets/enemies/kobold_crystal/kobold_crystal.png");
    SetChannelingSprite("assets/enemies/kobold_crystal/kobold_crystal_channelling.png");

    SetDamage(0);
    SetMaxHP(750,true)
    SetSpeed(0)
    SetRange(32);
    SetInvincible(GetObjRef(),true)
    RemoveFromCount(GetObjRef(),true);

    crash = AddAbility(GetObjRef(),"assets/enemies/kobold_crystal/ability_crash.lua",0);


    SetAttackSounds({
        "assets/audio/attacks/ranged_magic/magic_1.wav",
        "assets/audio/attacks/ranged_magic/magic_2.wav",
        "assets/audio/attacks/ranged_magic/magic_3.wav",
        "assets/audio/attacks/ranged_magic/magic_4.wav",
        "assets/audio/attacks/ranged_magic/magic_5.wav",
        "assets/audio/attacks/ranged_magic/magic_6.wav"
    });

end

function update(dt)
    if (AnyObjInCombat()) then
        CastAbility(crash,6,{{target=GetRandomUnit(TYPE_ENEMY,Band(Bnot(TYPE_TANK),TYPE_ALL),256)[1]}});
    end
end

function kill()

end