local attackSpeedMin = 2
local attackSpeedMax = 0.25

local attackSpeed = attackSpeedMin
local lastAttacked = -1

local maxHP = 250;

function setup()
    SetSprite("assets/enemies/berzerker/berzerker.png");

    SetDamage(5);
    SetMaxHP(maxHP,true)
    SetSpeed(80)
    SetRange(40);
    SetMaxHP(400,true)
    SetAttackSpeed(GetObjRef(),2)

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

function update(dt)
end

function kill()

end

function onattack(obj)
    if lastAttacked == obj then
        attackSpeed = attackSpeed - 0.1
        if (attackSpeed < attackSpeedMax) then
            attackSpeed = attackSpeedMax
        end
        SetAttackSpeed(GetObjRef(),attackSpeed);    
    else 
        lastAttacked = obj
        attackSpeed = attackSpeedMin
        SetAttackSpeed(GetObjRef(),attackSpeed);    
    end
end