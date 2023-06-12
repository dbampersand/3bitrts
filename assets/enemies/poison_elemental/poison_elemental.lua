local nova = -1
local acidPool = -1

local novaTimer = 6
local acidPoolTimer = RandRange(1,2)
function setup()
    SetSprite("assets/enemies/poison_elemental/poison_elemental.png");

    SetDamage(30);
    SetMaxHP(1500,true)
    SetSpeed(120)
    SetRange(48);

    nova = AddAbility(GetObjRef(),"assets/enemies/poison_elemental/ability_nova.lua",0);
    acidPool = AddAbility(GetObjRef(),"assets/enemies/poison_elemental/ability_acid_pool.lua",1);


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
    if (IsInCombat()) then

        novaTimer = novaTimer - dt;
        if (novaTimer <= 0) then
            CastAbility(nova,0.25,{});
        end

        acidPoolTimer = acidPoolTimer - dt;
        if (acidPoolTimer <= 0) then
            CastAbility(acidPool,1,{});
        end
    end
end


function kill()

end