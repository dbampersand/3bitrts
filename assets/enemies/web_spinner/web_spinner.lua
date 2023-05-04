local targ = -1
local attacking = false

local timer = 0
local timeBetweenShots = 0.1

local spinWeb = -1

local moving = false
local moveTimer = 0
local timeToMove = 0.4

local actionTimer = RandRange(1,2.5)

local atk = -1

function setup()
    SetSprite("assets/enemies/web_spinner/web_spinner.png");

    SetDamage(20);
    SetMaxHP(1200,true)
    SetSpeed(80)
    SetRange(50);

    spinWeb = AddAbility(GetObjRef(),"assets/enemies/web_spinner/ability_spin_web.lua",0);

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

end

function update(dt)
    if (IsInCombat()) then
        actionTimer = actionTimer - dt;
        if (actionTimer <= 0) then

            if (GetAbilityCooldown(GetObjRef(),spinWeb) < timeToMove and GetAbilityCooldown(GetObjRef(),spinWeb) > 0) then
                moving = true
            else
                local castTarget = GetRandomUnit(TYPE_ENEMY,TYPE_ANY,GetAbilityRange(GetObjRef(),spinWeb),1)[1];
                atk = CastAbility(spinWeb,1,{{target = castTarget}});
            end
            
            if (moving) then
                local target = GetAttackTarget(GetObjRef())
                local newMove = GetHeadingVector(GetX(target),GetY(target),GetX(GetObjRef()),GetY(GetObjRef()))
                local x = GetX(GetObjRef()) + (newMove.headingx * 100)
                local y = GetY(GetObjRef()) + (newMove.headingy * 100)

                SetMovePoint(GetObjRef(),x,y,false,true);
                
                if (GetAbilityCooldown(GetObjRef(),spinWeb) <= 0) then
                    moving = false;
                    local castTarget = GetRandomUnit(TYPE_ENEMY,TYPE_ANY,GetAbilityRange(GetObjRef(),spinWeb),1)[1];
                    atk = CastAbility(spinWeb,1,{{target = castTarget}});
                end
            end
        end
    end
end

function kill()
end