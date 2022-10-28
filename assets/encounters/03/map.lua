

function setup()
    SetMapSprite("assets/encounters/03/map.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/enemies/cusith/cusith.lua",120,22,TYPE_ENEMY,5);

    CreateObject("assets/decor/pillar24x24.lua",66,70,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",181,70,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",66,150,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",181,150,TYPE_DECORATION,0);


end

function update(dt)
end

function kill()

end