local passion = -1
local burn = -1

function setup()
    SetSprite("assets/summoned/woven_imp/woven_imp.png");

    SetDamage(30);
    SetMaxHP(800,true)
    SetSpeed(60)
    SetRange(64);

    passion = AddAbility(GetObjRef(),"assets/summoned/woven_imp/ability_passion.lua",0);
    burn = AddAbility(GetObjRef(),"assets/summoned/woven_imp/ability_burn.lua",1);

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