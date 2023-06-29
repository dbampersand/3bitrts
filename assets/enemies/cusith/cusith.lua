
local maxHP = 8000;

local howl = 0
local bite = 0
local callPack = 0
local spin = 0
local focus = 0
local fangs

local howlTimer = 0
local howlCD = 30

function setup()
    SetSprite("assets/enemies/cusith/cusith.png");
    SetChannelingSprite("assets/enemies/cusith/cusith_channelling.png")
    SetDamage(30);
    SetMaxHP(maxHP,true)
    SetSpeed(80)
    SetRange(45);
    SetAttackSpeed(GetObjRef(),1)
    SetObjIsBoss(GetObjRef(),true);

    howl = AddAbility(GetObjRef(),"assets/enemies/cusith/ability_howl.lua",0)   
    bite = AddAbility(GetObjRef(),"assets/enemies/cusith/ability_bite.lua",1);
    callPack = AddAbility(GetObjRef(),"assets/enemies/cusith/ability_call_pack.lua",2);
    spin = AddAbility(GetObjRef(),"assets/enemies/cusith/ability_spin.lua",3);
    focus = AddAbility(GetObjRef(),"assets/enemies/cusith/ability_focus.lua",4);
    fangs = AddAbility(GetObjRef(),"assets/enemies/cusith/ability_fangs.lua",5);

    SetAttackSounds({
        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });


end 
function untoggle()

end
function update(dt)

    if (IsInCombat()) then
        howlTimer = howlTimer + dt

        if (howlTimer > howlCD) then
            SetMovePoint(128,128);
            CastAbility(howl,3,{});
            SetAbilityCooldownTimer(GetObjRef(),howl,0);
            if (howlTimer > howlCD + GetAbilityCooldown(GetObjRef(),howl)*3) then
                howlTimer = 0
            end
            do return end;
        end

        CastAbility(callPack,3,{})
        CastAbility(bite,1,{{target = GetHighestThreat()}})
        CastAbility(spin,2,{});
        if (GetHP(GetObjRef()) < 0.75 * GetMaxHP(GetObjRef())) then
            CastAbility(fangs,1,{{target = GetAttackTarget(GetObjRef())}});
        end
        if (GetHP(GetObjRef()) < 0.4 * GetMaxHP(GetObjRef())) then
            CastAbility(focus,0,{});
        end
    end
end

function kill()
end

function onattack(obj)

end
