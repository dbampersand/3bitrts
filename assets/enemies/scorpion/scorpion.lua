local spit = 0

function setup()
    SetSprite("assets/enemies/scorpion/scorpion.png");
    SetChannelingSprite("assets/enemies/scorpion/scorpion_channeling.png")

    SetDamage(20);
    SetMaxHP(750,true)
    SetSpeed(80)
    SetRange(50);

    spit = AddAbility(GetObjRef(),"assets/enemies/scorpion/ability_spit.lua",0);
   
    SetAttackSounds({
        "assets/audio/attacks/attack_spit/spit_1.wav",
        "assets/audio/attacks/attack_spit/spit_2.wav",
        "assets/audio/attacks/attack_spit/spit_3.wav",
        "assets/audio/attacks/attack_spit/spit_4.wav",
        "assets/audio/attacks/attack_spit/spit_5.wav",
    });

end

function update(dt)
    if (IsInCombat()) then

        local target = {};

        local targ = GetRandomUnit(TYPE_ENEMY,TYPE_ANY,999)[1];

        target["target"] = targ;
        if (target["target"] ~= nil) then
            CastAbility(spit,1,{target});
        end
    end
end

function kill()

end