
function setup()
    AbilitySetPortrait("Assets/friendly/Rogue/icon_dash.png");
    AbilitySetCastType(ABILITY_POINT);
    SetAbilityRange(120)
    SetDescription("Dash\n\nDashes through enemies, dealing damage.")
    SetAbilityTargetHint(HINT_LINE)
end
function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 5;  


    local objsHit = GetBetween(GetObjRef(),x,y);
    for i=0, #objsHit do

        if (GetOwnedBy(objsHit[i]) ~= GetOwnedBy(GetObjRef())) then
            Print("gg" ..  objsHit[i]);
            ApplyEffect(objsHit[i],{f1});
        end
    end
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end