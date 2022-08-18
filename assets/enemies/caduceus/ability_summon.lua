function setup()
    SetAbilityRange(30)
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Summon\n\nSummons three vipers.")
    AbilitySetCastType(ABILITY_INSTANT);

end
function casted(x,y,obj,headingx,headingy)
    local cx = GetX(GetObjRef());
    local cy = GetY(GetObjRef());

    local point = {x=cx-30, y=cy};
    for i=1,3 do
        CreateObject("assets/enemies/viper/viper.lua",point.x,point.y,TYPE_ENEMY,2);
        point = RotatePoint(point.x,point.y,cx,cy,45);
    end
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end