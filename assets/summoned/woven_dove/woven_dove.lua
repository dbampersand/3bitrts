local cure = -1
local spirited_aura = -1

function setup()
    SetSprite("assets/summoned/woven_dove/woven_dove.png");

    SetDamage(30);
    SetMaxHP(800,true)
    SetSpeed(60)
    SetRange(64);

    cure = AddAbility(GetObjRef(),"assets/summoned/woven_dove/ability_cure.lua",0);
    spirited_aura = AddAbility(GetObjRef(),"assets/summoned/woven_dove/ability_spirited_aura.lua",1);

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