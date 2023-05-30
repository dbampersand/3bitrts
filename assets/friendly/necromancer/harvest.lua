local radius = 30
local aoe = -1;

function setup()
    AbilitySetCastType(ABILITY_INSTANT);

    SetAbilityRange(20)

    SetCooldown(99999999)
    SetCooldownTimer(99999999);

    SetAbilityMaxStacks(3)
    SetAbilityStacks(0)
    
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_heal.png");
    SetDescription("Summon Skeleotn\n\nSummons a skeleton, which can be ranged or melee.")

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/bard/audio/song_of_healing.wav",1)

    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    MoveAttack(attackRef,GetX(obj),GetY(obj));

end