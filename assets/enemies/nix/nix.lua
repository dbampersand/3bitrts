local lavaField = 0
local melt = 0;
local chaosBall = 0

function setup()
    SetSprite("assets/enemies/nix/nix.png");

    SetDamage(12);
    SetMaxHP(3000,true)
    SetSpeed(120)
    SetRange(32);

    melt = AddAbility(GetObjRef(),"assets/enemies/nix/ability_melt.lua",0);
    lavaField = AddAbility(GetObjRef(),"assets/enemies/nix/ability_lava_field.lua",1);
    chaosBall = AddAbility(GetObjRef(),"assets/enemies/nix/ability_chaos_ball.lua",2);

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
        local randUnit = {target = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,256)};
        --CastAbility(lavaField,2,{});
        --CastAbility(melt,1,{{target=GetAttackTarget(GetObjRef())}});
        CastAbility(chaosBall,0.25,{{target=GetAttackTarget(GetObjRef())}});

    end
end

function kill()

end