local dropBomb = -1

local movePoint = {}
local moveTimer = 0

local castTimer = 1
local timeBetweenCasts = 0.5

function setup()
    SetSprite("assets/enemies/kobold_saboteur/kobold_saboteur.png");

    SetDamage(15);
    SetMaxHP(750,true)
    SetSpeed(120)
    SetRange(128);

    dropBomb = AddAbility(GetObjRef(),"assets/enemies/kobold_saboteur/ability_drop_bomb.lua",0);


    SetAttackSounds({
        "assets/audio/attacks/melee_hammer/hammer_1.wav",
        "assets/audio/attacks/melee_hammer/hammer_2.wav",
        "assets/audio/attacks/melee_hammer/hammer_3.wav",
        "assets/audio/attacks/melee_hammer/hammer_4.wav",
        "assets/audio/attacks/melee_hammer/hammer_5.wav",
        "assets/audio/attacks/melee_hammer/hammer_6.wav"
    });

end

function update(dt)
    if (IsInCombat()) then
        local enemyUnitIsClose = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,32);
        if (moveTimer <= 0 and GetStacks(GetObjRef(),dropBomb) >= GetMaxStacks(GetObjRef(),dropBomb) and #enemyUnitIsClose > 0) then
            movePoint.x = RandRange(0,GetMapWidth());
            movePoint.y = RandRange(0,GetMapHeight())
            castTimer = 0;

            moveTimer = timeBetweenCasts * GetMaxStacks(GetObjRef(),dropBomb)

        end
        if (moveTimer > 0) then
            moveTimer = moveTimer - dt
            castTimer = castTimer - dt;

            SetMovePoint(GetObjRef(),movePoint.x,movePoint.y,false,true);
        end
        if (castTimer <= 0 and moveTimer >= 0) then
            castTimer = timeBetweenCasts
            CastAbility(dropBomb,0,{x = GetX(GetObjRef()),y = GetY(GetObjRef())})
        end
    end
end


function kill()

end