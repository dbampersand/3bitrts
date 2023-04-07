local large = 0
local small = 0

local timer = 0;
local maxDecisionTime = 2;
local minDecisionTime = 0.2
function setup()
    SetSprite("assets/enemies/rock_tosser/rock_tosser.png");

    SetDamage(10);
    SetMaxHP(600,true)
    SetSpeed(80)
    SetRange(60);

    large = AddAbility(GetObjRef(),"assets/enemies/rock_tosser/ability_throw_large_rock.lua",0);
    small = AddAbility(GetObjRef(),"assets/enemies/rock_tosser/ability_throw_small_rock.lua",1);

    SetChannelingSprite("assets/enemies/rock_tosser/rock_tosser_channeling.png")

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
    if (IsInCombat()) then
        timer = timer - dt;
        if (timer < 0) then
            timer = RandRange(minDecisionTime,maxDecisionTime)

            local target = {};

            local targ = GetRandomUnit(TYPE_ENEMY,TYPE_ANY,999)[1];
            

            target["target"] = targ;
            if (target["target"] ~= nil) then
                CastAbility(large,3,{target});
                CastAbility(small,4,{target});

            end
        end
    end
end

function kill()

end