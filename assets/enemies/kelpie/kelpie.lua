local charge = -1

function setup()
    SetSprite("assets/enemies/kelpie/kelpie.png");

    SetDamage(40);
    SetMaxHP(500,true)
    SetSpeed(120)
    SetRange(5);

    charge = AddAbility(GetObjRef(),"assets/enemies/kelpie/ability_charge.lua",0);


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
        CastAbility(charge,1.5,{{target = GetRandomUnit(TYPE_ENEMY,Bor(Bor(TYPE_HEALER,TYPE_RANGEDDPS),TYPE_MELEEDPS),999)[1]}})
    end
end

function kill()

end