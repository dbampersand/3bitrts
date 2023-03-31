local quartz = -1
local time = 0

local isInIntermission = false

function setup()
    SetSprite("assets/enemies/nuul/nuul.png");

    SetDamage(12);
    SetMaxHP(3000,true)
    SetSpeed(0)
    SetRange(32);   

    quartz = AddAbility(GetObjRef(),"assets/enemies/nuul/ability_quartz.lua",0);

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
    local hpPercent = GetHPPercent(GetObjRef());
    if (hpPercent < 60 and hpPercent > 30 and isInIntermission == false) then
        isInIntermission = true
        CastAbility(quartz,{});
    end

end

function kill()

end