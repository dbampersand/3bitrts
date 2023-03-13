local sunDisc = 0

function setup()
    SetSprite("assets/enemies/wadjet/wadjet.png");

    SetDamage(20);
    SetMaxHP(1500,true)
    SetSpeed(80)
    SetRange(5);

    sunDisc = AddAbility(GetObjRef(),"assets/enemies/wadjet/ability_sun_disc.lua",0);

    SetAttackSounds({
        "assets/audio/attacks/ranged_magic/magic_1.wav",
        "assets/audio/attacks/ranged_magic/magic_2.wav",
        "assets/audio/attacks/ranged_magic/magic_3.wav",
        "assets/audio/attacks/ranged_magic/magic_4.wav",
        "assets/audio/attacks/ranged_magic/magic_5.wav",
        "assets/audio/attacks/ranged_magic/magic_6.wav"
    });

end

function update(dt)
    local threatlist = GetThreatRank();
    local target = {};
    target["target"] = threatlist[1];
    if (target["target"] ~= nil) then
        if (GetStacks(GetObjRef(),sunDisc)==3) then
            CastAbility(0,0,{{target = threatlist[1]}});
            CastAbility(0,0,{{target = threatlist[2]}});
            CastAbility(0,0,{{target = threatlist[3]}});
        end
    end
end

function kill()

end