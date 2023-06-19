local tarTrap = -1
local shoot = -1

local movePoint = {}
local moveTimer = 0

function setup()
    SetSprite("assets/enemies/kobold_crossbow/kobold_crossbow.png");
    SetChannelingSprite("assets/enemies/kobold_crossbow/kobold_crossbow_cast_shoot.png")
    
    SetDamage(15);
    SetMaxHP(750,true)
    SetSpeed(120)
    SetRange(128);

    tarTrap = AddAbility(GetObjRef(),"assets/enemies/kobold_crossbow/ability_tar_trap.lua",0);
    shoot = AddAbility(GetObjRef(),"assets/enemies/kobold_crossbow/ability_shoot.lua",1);


    SetAttackSounds(
        {
        "assets/audio/attacks/ranged_bow/bow_1.wav",
        "assets/audio/attacks/ranged_bow/bow_2.wav",
        "assets/audio/attacks/ranged_bow/bow_3.wav",
        "assets/audio/attacks/ranged_bow/bow_4.wav",
        "assets/audio/attacks/ranged_bow/bow_5.wav",
        "assets/audio/attacks/ranged_bow/bow_6.wav",
        "assets/audio/attacks/ranged_bow/bow_7.wav",
        "assets/audio/attacks/ranged_bow/bow_8.wav"
    }
    );

end

function update(dt)
    if (IsInCombat()) then
        local enemyUnitIsClose = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,32);
        if (ObjIsChannelling(GetObjRef()) == false and moveTimer <= 0 and GetStacks(GetObjRef(),tarTrap) >= 1 and #enemyUnitIsClose > 0) then
            CastAbility(tarTrap,0.25,{{x=GetX(GetObjRef()),y=GetY(GetObjRef())}});
            movePoint.x = RandRange(0,GetMapWidth());
            movePoint.y = RandRange(0,GetMapHeight())
            moveTimer = 1.5
        end
        if (moveTimer > 0) then
            moveTimer = moveTimer - dt
            SetMovePoint(GetObjRef(),movePoint.x,movePoint.y,false,true);
        end
        if (moveTimer <= 0) then
            CastAbility(shoot,1,{{target=GetRandomUnit(TYPE_ENEMY,TYPE_HEALER,GetAbilityRange(GetObjRef(),shoot),1)[1]}})
        end
    end
end


function kill()

end