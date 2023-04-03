local quartz = -1
local void = -1

local isInIntermission = false
local intermissionStart = 60
local intermissionEnd = 30

local timer = 0
function setup()
    SetSprite("assets/enemies/nuul/nuul.png");

    SetDamage(50);
    SetMaxHP(6000,true)
    SetSpeed(0)
    SetRange(128);   

    quartz = AddAbility(GetObjRef(),"assets/enemies/nuul/ability_quartz.lua",0);
    void = AddAbility(GetObjRef(),"assets/enemies/nuul/ability_void.lua",1);
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
    if (IsInCombat(GetObjRef())) then
        timer = timer + dt;

        local hpPercent = GetHPPercent(GetObjRef());
        if (hpPercent < intermissionStart and hpPercent > intermissionEnd) then
            if (isInIntermission == false) then
                isInIntermission = true
                CastAbility(quartz,{});
            end
            do return end;
        end
        if (hpPercent <= intermissionEnd and isInIntermission == true) then
            isInIntermission = false
            CastAbility(quartz,{});
        end
        if (timer > 4) then
            CastAbility(void,{});
        end
    end

end

function kill()

end