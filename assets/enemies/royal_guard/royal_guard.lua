local charge = 0

function setup()
    SetSprite("assets/enemies/royal_guard/royal_guard.png");
    SetChannelingSprite("assets/enemies/royal_guard/royal_guard_channelling.png");

    SetDamage(7);
    SetMaxHP(750,true)
    SetSpeed(120)
    SetRange(6);

    charge = AddAbility(GetObjRef(),"assets/enemies/royal_guard/ability_charge.lua",0);

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
        local randUnit = {target = GetRandomUnit(TYPE_ENEMY,Band(TYPE_ALL,Bnot(TYPE_TANK),256))[1]};
        CastAbility(charge,2,{randUnit});
    end
end

function kill()

end