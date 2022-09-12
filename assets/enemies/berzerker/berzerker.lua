local attackSpeed = 2
function setup()
    SetSprite("assets/enemies/berzerker/berzerker.png");

    SetDamage(5);
    SetMaxHP(600,true)
    SetSpeed(80)
    SetRange(40);
    SetAttackSpeed(2)


end

function update(dt)
end

function kill()

end

function OnAttack(obj)
    attackSpeed = attackSpeed - 0.1
    SetAttackSpeed(attackSpeed);    
end