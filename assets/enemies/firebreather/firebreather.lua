local spit = 0

local timer = 0;
local maxDecisionTime = 6;
local minDecisionTime = 3
function setup()
    SetSprite("assets/enemies/firebreather/firebreather.png");
    SetChannelingSprite("assets/enemies/firebreather/firebreather_channelling.png");

    SetDamage(15);
    SetMaxHP(600,true)
    SetSpeed(80)
    SetRange(60);

    spit = AddAbility(GetObjRef(),"assets/enemies/firebreather/ability_spit.lua",0);
    breathe = AddAbility(GetObjRef(),"assets/enemies/firebreather/ability_breathe.lua",1);

    SetAttackSounds({
        "assets/audio/attacks/attack_spit/spit_1.wav",
        "assets/audio/attacks/attack_spit/spit_2.wav",
        "assets/audio/attacks/attack_spit/spit_3.wav",
        "assets/audio/attacks/attack_spit/spit_4.wav",
        "assets/audio/attacks/attack_spit/spit_5.wav",
    });
    timer = RandRange(minDecisionTime,maxDecisionTime)
end

function update(dt)
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