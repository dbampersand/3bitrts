
local terror = -1
local timeBetweenCasts = 0.2

local casting = false
local timer = timeBetweenCasts

function setup()
    SetSprite("assets/enemies/ghost/ghost.png");
    SetChannelingSprite("assets/enemies/ghost/ghost_channelling.png");

    SetDamage(60);
    SetMaxHP(1200,true)
    SetSpeed(60)
    SetRange(80);

    terror = AddAbility(GetObjRef(),"assets/enemies/ghost/ability_terror.lua",0);

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
        if (GetAbilityStacks(GetObjRef(),terror) == GetMaxStacks(GetObjRef(),terror)) then
            casting = true
        end
        if (GetAbilityStacks(GetObjRef(),terror) == 0) then
            casting = false
        end
        if (casting) then
            timer = timer - dt
            if (timer < 0) then
                CastAbility(terror,0.2,{})
                timer = timeBetweenCasts
            end
        end
    end
end

function kill()

end