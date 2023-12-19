local distFromMax = 50

function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(16); 
    AbilitySetPortrait("assets/enemies/sea_witch/ability_spawn_tentacle.png");
    SetDescription("[b]Spawn Tentacle\n\nCreates multiple tentacles around the caster.")
end
function casted(x,y,obj,headingx,headingy)
    local parentAggroGroup = GetAggroGroup(GetObjRef());
    for i = 1, 2 do
        local point = {}
        point.x = GetX(GetObjRef())+RandRange(-distFromMax,distFromMax)
        point.y = GetY(GetObjRef())+RandRange(-distFromMax,distFromMax)

        if (i==1) then 
            SetAggroGroup(CreateObject("assets/enemies/tentacle_healer/tentacle_healer.lua",point.x,point.y,TYPE_FRIENDLY,2.5),parentAggroGroup);
        else
            SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",point.x,point.y,TYPE_FRIENDLY,2.5),parentAggroGroup);
        end
        PlaySound("assets/enemies/sea_witch/audio/ability_summon_tentacle.wav",0.5);


    end
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
end

function applyattack(atk,x,y)
end
function timerbeforetick(t,func)
end