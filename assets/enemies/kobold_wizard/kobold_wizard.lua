local bomb = 0

function setup()
    SetSprite("assets/enemies/kobold_miner/kobold_miner.png");

    SetDamage(7);
    SetMaxHP(750,true)
    SetSpeed(120)
    SetRange(6);

    fireball = AddAbility(GetObjRef(),"assets/enemies/kobold_wizard/ability_fireball.lua",0);
    burn = AddAbility(GetObjRef(),"assets/enemies/kobold_wizard/ability_burn.lua",1);


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
        local randUnit = {target = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,256)};
        CastAbility(fireball,2,{randUnit});
        CastAbility(burn,1,{{target=GetRandomUnit(TYPE_ENEMY,TYPE_TANK,256)}});

    end
end

function kill()

end