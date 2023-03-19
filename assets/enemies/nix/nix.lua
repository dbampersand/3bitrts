local lavaField = -1
local melt = -1
local searingBlast = -1
local lightningOrb = -1

function setup()
    SetSprite("assets/enemies/nix/nix.png");

    SetDamage(12);
    SetMaxHP(3000,true)
    SetSpeed(120)
    SetRange(32);

    melt = AddAbility(GetObjRef(),"assets/enemies/nix/ability_melt.lua",0);
    lavaField = AddAbility(GetObjRef(),"assets/enemies/nix/ability_lava_field.lua",1);
    searingBlast = AddAbility(GetObjRef(),"assets/enemies/nix/ability_searing_blast.lua",2);
    lightningOrb = AddAbility(GetObjRef(),"assets/enemies/nix/ability_lightning_orb.lua",3);

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
        CastAbility(melt,1,{{target=GetAttackTarget(GetObjRef())}});
        CastAbility(lavaField,2,{});
        if (TimeSinceLastCast(lavaField) < 2) then
            CastAbility(searingBlast,0.25,{{target=GetAttackTarget(GetObjRef())}});
        end
        CastAbility(lightningOrb,0.25,{{target=GetRandomUnit(TYPE_ENEMY,TYPE_ALL,256)}});

    end
end

function kill()

end