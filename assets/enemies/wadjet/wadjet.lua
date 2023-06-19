local sunDisc = -1
local fire =  -1

local timer = 0; 

function setup()
    SetSprite("assets/enemies/wadjet/wadjet.png");

    SetDamage(20);
    SetMaxHP(1500,true)
    SetSpeed(80)
    SetRange(5);

    sunDisc = AddAbility(GetObjRef(),"assets/enemies/wadjet/ability_sun_disc.lua",0);
    fire = AddAbility(GetObjRef(),"assets/enemies/wadjet/ability_fire.lua",1);
    SetAttackSounds({
        "assets/audio/attacks/ranged_magic/magic_1.wav",
        "assets/audio/attacks/ranged_magic/magic_2.wav",
        "assets/audio/attacks/ranged_magic/magic_3.wav",
        "assets/audio/attacks/ranged_magic/magic_4.wav",
        "assets/audio/attacks/ranged_magic/magic_5.wav",
        "assets/audio/attacks/ranged_magic/magic_6.wav",

        "assets/audio/attacks/attack_spit/spit_1.wav",
        "assets/audio/attacks/attack_spit/spit_2.wav",
        "assets/audio/attacks/attack_spit/spit_3.wav",
        "assets/audio/attacks/attack_spit/spit_4.wav",
        "assets/audio/attacks/attack_spit/spit_5.wav"
    });
end

function update(dt)
    local threatlist = GetThreatRank();
    local target = {};
    target["target"] = threatlist[1];

    if (IsInCombat()) then
        timer = timer + dt;
        if (timer > 5) then
            CastAbility(fire,1,{{}})
            if (target["target"] ~= nil) then
                if (GetStacks(GetObjRef(),sunDisc)==3) then
                    CastAbility(sunDisc,0,{{target = threatlist[1]}});
                    CastAbility(sunDisc,0,{{target = threatlist[2]}});
                    CastAbility(sunDisc,0,{{target = threatlist[3]}});
                end
            end
        end
    
    end

end

function kill()

end