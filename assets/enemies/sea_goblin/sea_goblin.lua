local sea_foam = -1

function setup()
    SetSprite("assets/enemies/sea_goblin/sea_goblin.png");

    SetDamage(15);
    SetMaxHP(500,true)
    SetSpeed(70)
    SetRange(32);

    sea_foam = AddAbility(GetObjRef(),"assets/enemies/sea_goblin/ability_sea_foam.lua",0);


    SetAttackSounds({
        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });

end

function update(dt)
    if (IsInCombat()) then
        CastAbility(sea_foam,2,{})
    end
end

function kill()

end