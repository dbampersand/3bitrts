local hellfire = -1
local curse = -1

function setup()
    SetSprite("assets/summoned/woven_demon/woven_demon.png");

    SetDamage(50);
    SetMaxHP(800,true)
    SetSpeed(60)
    SetRange(64);

    hellfire = AddAbility(GetObjRef(),"assets/summoned/woven_demon/ability_hellfire.lua",0);
    curse = AddAbility(GetObjRef(),"assets/summoned/woven_demon/ability_curse.lua",1);

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