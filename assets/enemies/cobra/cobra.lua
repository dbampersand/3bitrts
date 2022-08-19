function setup()
    SetSprite("assets/enemies/viper/viper.png");

    SetDamage(40);
    SetMaxHP(200,true)
    SetSpeed(80)

    AddAbility("assets/enemies/cobra/ability_spit.lua",0);

end
local timer = 0
function update(dt)
    timer = timer + dt;
    if (timer > 1) then
        timer = 0
        if (RandRange(0,5) <= 4) then
            do return end;
        end
    end
    local threatlist = GetThreatRank();
    local target = {};
    target["target"] = threatlist[1];
    if (target["target"] ~= nil) then
        CastAbility(0,4,{target});
    end
end

function kill()

end