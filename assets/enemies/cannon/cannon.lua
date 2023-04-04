local shoot = 0
local timer = 0

function setup()
    SetSprite("assets/enemies/cannon/cannon.png");

    SetDamage(0);
    SetMaxHP(750,true)
    SetSpeed(0)
    SetRange(60);
    SetInvincible(GetObjRef(),true)
    RemoveFromCount(GetObjRef(),true);
    SetDecoration(GetObjRef(),true);

    shoot = AddAbility(GetObjRef(),"assets/enemies/cannon/ability_shoot.lua",0);


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
    timer = timer - dt;
    if (timer < 0) then
        timer = RandRange(0.25,2)
        local target = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,GetAbilityRange(GetObjRef(),shoot))[1];
        if (target ~= nil) then
            local x = GetX(target) + RandRange(-10,10);
            local y = GetY(target) + RandRange(-10,10);

            CastAbility(shoot,4,{{x = x, y = y}});
        end
    end
end

function kill()

end