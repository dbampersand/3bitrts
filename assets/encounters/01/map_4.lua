
function setup()
    SetMapSprite("assets/encounters/01/map_4.png")

    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",168,123,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",120,140,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",120,150,TYPE_ENEMY,0),1);
  

    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",200,260,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",220,260,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",240,255,TYPE_ENEMY,0),2);

    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",240,340,TYPE_ENEMY,0),3);
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",280,345,TYPE_ENEMY,0),3);

    SetAggroGroup(CreateObject("assets/enemies/naja/naja.lua",146,430,TYPE_ENEMY,0),4);
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",170,440,TYPE_ENEMY,0),4);
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",180,430,TYPE_ENEMY,0),4);

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