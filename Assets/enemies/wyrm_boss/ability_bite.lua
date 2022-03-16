function setup()

end

function casted(x,y,obj,headingx,headingy)
    if (obj == GetThisObj()) then
        return;
    end
    Print("GGGG")

    f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_DAMAGE;
    f1["numTriggers"] = 5
    f1["value"] = 2;  
    f1["duration"] = 10;

    ApplyEffect(obj,{f1});
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end