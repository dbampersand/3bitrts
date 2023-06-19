
local rain = -1

function setup()
    SetSprite("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.png");
    SetChannelingSprite("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged_channelling.png");

    SetDamage(35);
    SetMaxHP(350,true)
    SetSpeed(80)
    SetRange(140);

    rain = AddAbility(GetObjRef(),"assets/enemies/skeleton_minion_ranged/ability_rain.lua",0);

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
    if (IsInCombat()) then
        CastAbility(rain,1,{{target = GetAttackTarget(GetObjRef()) }} )

    end
end

function kill()

end