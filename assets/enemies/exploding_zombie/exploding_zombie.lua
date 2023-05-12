local targ = -1

local explode = -1

local indicator = -1

function setup()
    SetSprite("assets/enemies/exploding_zombie/exploding_zombie.png");

    SetDamage(1);
    SetMaxHP(400,true)
    SetSpeed(20)
    SetRange(10);

    explode = AddAbility(GetObjRef(),"assets/enemies/exploding_zombie/ability_explode.lua",0);



    SetAttackSounds({
        "assets/audio/attacks/attack_spit/spit_1.wav",
        "assets/audio/attacks/attack_spit/spit_2.wav",
        "assets/audio/attacks/attack_spit/spit_3.wav",
        "assets/audio/attacks/attack_spit/spit_4.wav",
        "assets/audio/attacks/attack_spit/spit_5.wav",

        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });
    SetObjectPush(true)
    indicator = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", GetHintRadius(GetObjRef(),explode), 9999, 9999, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, -1, {})


end
function latesetup()

end
function update(dt)
    if (IsInCombat() and IsAlive(targ) == false) then
        targ = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,9999,1)[1]
        Print(targ)
    end
    if (IsAlive(targ)) then
       SetMovePoint(GetObjRef(),GetX(targ),GetY(targ),true,true); 
       SetAttackTarget(GetObjRef(),targ);
    end
    if (IsInCombat() and targ >= 0) then
        if (IsInAttackRange(GetObjRef(),targ)) then
            CastAbility(explode,0,{})
        end
    end

    MoveAttack(indicator,GetX(GetObjRef()),GetY(GetObjRef()))

end

function kill()
    RemoveAttack(indicator)
end