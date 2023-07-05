local peel = -1

local moveTime = 3
local timeToMove = 8
local timer = timeToMove

local STATE_DEFAULT = 0
local STATE_RUNNING = 1

local state = STATE_DEFAULT
local movePoint = {}

function setup()
    SetSprite("assets/enemies/spriggan/spriggan.png");

    SetDamage(15);
    SetMaxHP(500,true)
    SetSpeed(70)
    SetRange(32);

    peel = AddAbility(GetObjRef(),"assets/enemies/spriggan/ability_peel.lua",0);


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
        timer = timer + dt

        if (state == STATE_DEFAULT and timer >= timeToMove) then
            state = STATE_RUNNING
            local target = GetAttackTarget(GetObjRef())
            local dir = GetHeadingVector(GetX(target),GetY(target),GetX(GetObjRef()),GetY(GetObjRef()))
            movePoint.x = GetX(GetObjRef()) + (dir.headingx*50)
            movePoint.y = GetY(GetObjRef()) + (dir.headingy*50)
                
            timer = 0
        end
        if (state == STATE_RUNNING) then
            CastAbility(peel,0,{{target = GetAttackTarget(GetObjRef())}} )
            SetMovePoint(GetObjRef(),movePoint.x,movePoint.y,false,false)

            if (timer > moveTime) then
                state = STATE_DEFAULT
                timer = 0
            end
        end
    end
end

function kill()

end