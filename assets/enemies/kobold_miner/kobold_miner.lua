local bomb = 0

function setup()
    SetSprite("assets/enemies/kobold_miner/kobold_miner.png");
    SetChannelingSprite("assets/enemies/kobold_miner/kobold_miner_channelling.png")

    SetDamage(7);
    SetMaxHP(750,true)
    SetSpeed(120)
    SetRange(6);

    bomb = AddAbility(GetObjRef(),"assets/enemies/kobold_miner/ability_throw_bomb.lua",0);
   
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
        local randUnit = {target = GetRandomUnit(TYPE_ENEMY,Band(TYPE_ALL,Bnot(TYPE_TANK)),256)[1]};
        CastAbility(bomb,2,{randUnit});
    end
end

function kill()

end