
function setup()
    SetMapSprite("assets/encounters/01/map_4.png")

    CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",168,123,TYPE_ENEMY,0);
    
    CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",120,182,TYPE_ENEMY,0);
    CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",120,182,TYPE_ENEMY,0);
  
    CreateObject("assets/enemies/naja/naja.lua",250,340,TYPE_ENEMY,0);
    CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",250,340,TYPE_ENEMY,0);
    CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",250,340,TYPE_ENEMY,0);

    SetSpawnPoint(13,86)
    
end

function update(dt)
    --ChangeMap("assets/encounters/01/map_2.lua")
        
    --if (NumObjectsOwnedByPlayer(TYPE_ENEMY) == 0) then
      --  ChangeMap("assets/encounters/01/map_final.lua")
    --end

end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/01/map_final.lua")
    return false;
end