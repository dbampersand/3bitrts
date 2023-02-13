local squeeze = 0;
local acid = 0

local timer = 0;
function setup()
    SetSprite("assets/enemies/giant_ooze/giant_ooze.png");

    SetDamage(40);
    SetMaxHP(4500,true)
    SetSpeed(60)
    SetRange(10);

    squeeze = AddAbility(GetObjRef(),"assets/enemies/giant_ooze/ability_squeeze.lua",0);
    acid = AddAbility(GetObjRef(),"assets/enemies/giant_ooze/ability_acid.lua",1);

    SetAttackSounds({
        "assets/audio/attacks/attack_spit/spit_1.wav",
        "assets/audio/attacks/attack_spit/spit_2.wav",
        "assets/audio/attacks/attack_spit/spit_3.wav",
        "assets/audio/attacks/attack_spit/spit_4.wav",
        "assets/audio/attacks/attack_spit/spit_5.wav",
    });

end
function update(dt)
    timer = timer + dt;
    if (timer > 5) then
        CastAbility(squeeze,5,{});
    end
    local acidTarget = {};
    acidTarget["target"] = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,999);
    CastAbility(acid,3,{acidTarget})
end

function kill()
end

