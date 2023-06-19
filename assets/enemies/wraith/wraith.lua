local desolate = -1
local negative = -1

local negativeTimer = 6
local desolateTimer = RandRange(3,4)
function setup()
    SetSprite("assets/enemies/wraith/wraith.png");

    SetDamage(30);
    SetMaxHP(1500,true)
    SetSpeed(120)
    SetRange(48);

    desolate = AddAbility(GetObjRef(),"assets/enemies/wraith/ability_desolate.lua",0);
    negative = AddAbility(GetObjRef(),"assets/enemies/wraith/ability_negative.lua",1);


    SetAttackSounds({
        "assets/audio/attacks/ranged_magic/magic_1.wav",
        "assets/audio/attacks/ranged_magic/magic_2.wav",
        "assets/audio/attacks/ranged_magic/magic_3.wav",
        "assets/audio/attacks/ranged_magic/magic_4.wav",
        "assets/audio/attacks/ranged_magic/magic_5.wav",
        "assets/audio/attacks/ranged_magic/magic_6.wav"
    });

end

function update(dt)
    if (IsInCombat()) then

        desolateTimer = desolateTimer - dt;
        if (desolateTimer <= 0) then
            local targ = GetFurthestObjectInRange(GetX(GetObjRef()),GetY(GetObjRef()),256,TYPE_ENEMY);
            CastAbility(desolate,1,{{target = targ}});
        end

        negativeTimer = negativeTimer - dt;
        if (negativeTimer <= 0) then
            CastAbility(negative,2,{});
        end
    end
end


function kill()

end