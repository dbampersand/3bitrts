local sear = -1
local photon = -1

function setup()
    SetSprite("assets/summoned/ruby_star/ruby_star.png");

    SetDamage(30);
    SetMaxHP(800,true)
    SetSpeed(60)
    SetRange(64);

    sear = AddAbility(GetObjRef(),"assets/summoned/ruby_star/ability_sear.lua",0);
    photon = AddAbility(GetObjRef(),"assets/summoned/ruby_star/ability_photon.lua",1);

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