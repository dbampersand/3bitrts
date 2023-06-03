local radiance = -1
local lightningOrb = -1

function setup()
    SetSprite("assets/summoned/obsidian_demon/obsidian_demon.png");

    SetDamage(25);
    SetMaxHP(3000,true)
    SetSpeed(120)
    SetRange(32);

    lightningOrb = AddAbility(GetObjRef(),"assets/summoned/obsidian_demon/ability_lightning_orb.lua",0);
    radiance = AddAbility(GetObjRef(),"assets/summoned/obsidian_demon/ability_radiance.lua",1);

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