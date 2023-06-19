local raise = -1
local frostBlast = -1


function setup()
    SetSprite("assets/enemies/apprentice_lich/apprentice_lich.png");
    SetChannelingSprite("assets/enemies/apprentice_lich/apprentice_lich_casting_frost_blast.png");
    
    SetDamage(20);
    SetMaxHP(900,true)
    SetSpeed(80)
    SetRange(50);

    raise = AddAbility(GetObjRef(),"assets/enemies/apprentice_lich/ability_raise.lua",0);
    frostBlast = AddAbility(GetObjRef(),"assets/enemies/apprentice_lich/ability_frost_blast.lua",1);
    


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
    if (IsInCombat()) then
        CastAbility(raise,0,{{}});
        CastAbility(frostBlast,1,{{target = GetAttackTarget(GetObjRef()) }});


    end
end

function kill()

end