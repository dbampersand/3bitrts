local bite = -1
local wave = -1

local aoe = -1
local aoeDamageTotal = 20
local ticksPerSec = 2
local damageDuration = 30

function setup()
    SetSprite("assets/enemies/giant_worm/giant_worm.png");

    SetDamage(20);
    SetMaxHP(1600,true)
    SetSpeed(80)
    SetRange(50);

    bite = AddAbility(GetObjRef(),"assets/enemies/giant_worm/ability_bite.lua",0);
    wave = AddAbility(GetObjRef(),"assets/enemies/giant_worm/ability_wave.lua",1);

    SetAttackSounds({
        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });

    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = aoeDamageTotal * ticksPerSec / damageDuration;  
    f1["duration"] = damageDuration
    f1["triggersPerSecond"] = ticksPerSec;  
    f1["maxStacks"] = 12
    f1["canStack"] = true
    f1["name"] = "Fetid Stench"

    aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 60, 1, 9999,false,ATTACK_HITS_ENEMIES,COLOR_POISON,DITHER_DAMAGE_QUARTER,false, -1,0,0,0, {f1})
end

function update(dt)
    if (IsInCombat()) then
        CastAbility(bite,2,{{target = GetAttackTarget(GetObjRef())}});
        CastAbility(wave,1,{});
    end
    SetAttackLifetime(aoe,9999)
    SetAttackPosition(aoe,GetX(GetObjRef()),GetY(GetObjRef()))
end

function kill()
    RemoveAttack(aoe)
    aoe = -1
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_CURE;
    f1["value"] = 999;
    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",99999,0,0,false,ATTACK_HITS_ENEMIES,COLOR_HEAL,DITHER_HEAL_HALF,false,-1,0,0,0,{f1});

end