local spit = 0

local timer = 0;
local maxDecisionTime = 6;
local minDecisionTime = 3
function setup()
    SetSprite("assets/enemies/firebreather/firebreather.png");

    SetDamage(15);
    SetMaxHP(600,true)
    SetSpeed(80)
    SetRange(60);

    spit = AddAbility(GetObjRef(),"assets/enemies/firebreather/ability_spit.lua",0);
    breathe = AddAbility(GetObjRef(),"assets/enemies/firebreather/ability_breathe.lua",1);

    SetAttackSounds({
        "assets/audio/attacks/melee_hammer/hammer_1.wav",
        "assets/audio/attacks/melee_hammer/hammer_2.wav",
        "assets/audio/attacks/melee_hammer/hammer_3.wav",
        "assets/audio/attacks/melee_hammer/hammer_4.wav",
        "assets/audio/attacks/melee_hammer/hammer_5.wav",
        "assets/audio/attacks/melee_hammer/hammer_6.wav"
    });
    timer = RandRange(minDecisionTime,maxDecisionTime)
end

function update(dt)
    Print("gg");
    if (IsInCombat()) then
        if (timer < 2) then
            CastAbility(breathe,4,{{target=GetAttackTarget(GetObjRef())}});
        end
        timer = timer - dt;
        if (timer < 0) then
            CastAbility(spit,1,{{target=GetRandomUnit(TYPE_ENEMY,TYPE_ANY,999)[1]}});
        end
    end
end

function kill()

end