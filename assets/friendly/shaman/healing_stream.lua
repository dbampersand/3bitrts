local maxBounces = 4
local bounceDistance = 30
local maxHeal = 250

function setup()
    AbilitySetPortrait("assets/friendly/shaman/icon_healing_stream.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetAbilityRange(60)
    SetDescription("Healing Stream\n\nA heal that bounces between friendly targets.")
    SetManaCost(45)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/shaman/audio/healing_stream.wav",1);

    local newObj = obj
    local newX = 0; local newY = 0

    for i = 1, maxBounces do 

        --Apply effect
        local f1 = {};
        f1["trigger"] = TRIGGER_INSTANT
        f1["type"] = EFFECT_HEAL
        f1["value"] = maxHeal / maxBounces; 

        ApplyEffect(newObj,{f1})

        newX = GetX(newObj); newY = GetY(newObj);
        
        local objectsInRange = GetObjectsInRange(newX,newY,bounceDistance,GetObjFriendliness(newObj));
        table.sort(objectsInRange,function(a,b) return a.dist < b.dist end)
        for j = 1, #objectsInRange do
            if (objectsInRange[j].obj ~= newObj) then
                --bounce to the next (closest) object and break
                newObj = objectsInRange[j].obj;

                break;
            end
        end
    end
    return true;
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end