local throwDarts = 0

function setup()
    SetSprite("assets/enemies/kobold_foreman/kobold_foreman.png");

    SetDamage(7);
    SetMaxHP(750,true)
    SetSpeed(120)
    SetRange(6);

    throwDarts = AddAbility(GetObjRef(),"assets/enemies/kobold_foreman/ability_throw_darts.lua",0);

    SetAttackSounds({
        "assets/audio/attacks/melee_sword/sword_1.wav",
        "assets/audio/attacks/melee_sword/sword_2.wav",
        "assets/audio/attacks/melee_sword/sword_3.wav",
        "assets/audio/attacks/melee_sword/sword_4.wav",
        "assets/audio/attacks/melee_sword/sword_5.wav",
        "assets/audio/attacks/melee_sword/sword_6.wav",
        "assets/audio/attacks/melee_sword/sword_7.wav"
    });

end

function update(dt)
    if (IsInCombat()) then
        local randUnit = {target = GetRandomUnit(TYPE_ENEMY,Band(TYPE_ALL,Bnot(TYPE_TANK)),256)};
        CastAbility(throwDarts,2,{randUnit});
    end
end

function kill()

end