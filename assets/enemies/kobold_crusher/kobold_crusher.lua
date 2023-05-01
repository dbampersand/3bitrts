local charge = -1

function setup()
    SetSprite("assets/enemies/kobold_crusher/kobold_crusher.png");

    SetDamage(35);
    SetMaxHP(750,true)
    SetSpeed(120)
    SetRange(32);

    charge = AddAbility(GetObjRef(),"assets/enemies/kobold_crusher/ability_charge.lua",0);
    
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
    if (IsInCombat()) then
        local randUnit = {target = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,256)[1]};
        CastAbility(fireball,2,{randUnit});
        CastAbility(burn,1,{{target=GetRandomUnit(TYPE_ENEMY,TYPE_TANK,256)[1]}});

    end
end

function kill()

end