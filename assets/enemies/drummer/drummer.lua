
local attackSpeed = attackSpeedMin
local lastAttacked = -1

local maxHP = 250;
local aoe = -1
function setup()
    SetSprite("assets/enemies/drummer/drummer.png");

    SetDamage(5);
    SetMaxHP(maxHP,true)
    SetSpeed(80)
    SetRange(40);
    SetAttackSpeed(GetObjRef(),2)
    SetMaxHP(400,true)

    SetAttackSounds(
        {
        "assets/audio/attacks/ranged_bow/bow_1.wav",
        "assets/audio/attacks/ranged_bow/bow_2.wav",
        "assets/audio/attacks/ranged_bow/bow_3.wav",
        "assets/audio/attacks/ranged_bow/bow_4.wav",
        "assets/audio/attacks/ranged_bow/bow_5.wav",
        "assets/audio/attacks/ranged_bow/bow_6.wav",
        "assets/audio/attacks/ranged_bow/bow_7.wav",
        "assets/audio/attacks/ranged_bow/bow_8.wav"
    }
    );

end
function untoggle()

end
function update(dt)
    if (aoe == -1) then
        local f1 = {};
        f1["trigger"] = TRIGGER_CONST;
        f1["type"] = EFFECT_SPEED;
        --f1["numTriggers"] = 5
        f1["value"] = 10;
        f1["duration"] = 1.5;
        f1["triggersPerSecond"] = 1
        f1["name"] = "Speed"
        f1["overwrites"] = true
    
        local f2 = {};
        f2["trigger"] = TRIGGER_CONST;
        f2["type"] = EFFECT_ATTACKDAMAGE;
        f2["value"] = 2;
        f2["duration"] = 1.5;
        f2["triggersPerSecond"] = 1
        f2["name"] = "AttackDamage"
        f2["overwrites"] = true
        
        local f3 = {};
        f3["trigger"] = TRIGGER_CONST;
        f3["type"] = EFFECT_ATTACKSPEED;
        f3["value"] = -0.2;
        f3["duration"] = 1.5;
        f3["triggersPerSecond"] = 1
        f3["name"] = "AttackSpeed"
        f3["overwrites"] = true
    
        aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES,COLOR_SPEED,DITHER_HALF,true, -1, {f1,f2,f3})
    
    end
    if (aoe > -1) then
        MoveAttack(aoe,GetX(GetObjRef()),GetY(GetObjRef()));
        SetAttackLifetime(aoe, 10)
    end

end

function kill()
    RemoveAttack(aoe)
    aoe = -1
end

function OnAttack(obj)

end
