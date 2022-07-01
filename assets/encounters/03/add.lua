local maxHP = 400

function setup()
    SetSprite("assets/enemies/wyrm.png");

    SetDamage(10);
    SetMaxHP(maxHP,true)
    SetSpeed(10)
    SetObjectPush(true);
end

function update(dt)

end

function kill()

end