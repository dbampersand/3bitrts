local ivy = -1
local blossom = -1

function setup()
    SetSprite("assets/summoned/fairy/fairy.png");

    SetDamage(10);
    SetMaxHP(800,true)
    SetSpeed(60)
    SetRange(64);

    ivy = AddAbility(GetObjRef(),"assets/summoned/fairy/ability_ivy.lua",0);
    blossom = AddAbility(GetObjRef(),"assets/summoned/fairy/ability_blossom.lua",1);

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
end

function kill()

end