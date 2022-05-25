function setup()
    SetSprite("assets/enemies/trainingdummy/trainingdummy.png");

    SetDamage(0);
    SetMaxHP(9999,true)
    SetSpeed(0)
    SetObjectPush(true);
end

function update(dt)
    if (GetTime() % 10 == 0) then
        SetHP(9999);
    end
end

function kill()

end