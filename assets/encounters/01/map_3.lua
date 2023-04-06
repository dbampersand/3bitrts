
function setup()
    SetMapSprite("assets/encounters/01/map_3.png")

    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",58,99,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",58,99,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",58,82,TYPE_ENEMY,0),1);
    
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",89,294,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",96,301,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",150,303,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",177,315,TYPE_ENEMY,0),2);

    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",257,239,TYPE_ENEMY,0),3);
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",270,311,TYPE_ENEMY,0),3);

  
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",312,305,TYPE_ENEMY,0),4);
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",330,320,TYPE_ENEMY,0),4);

    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",240,220,TYPE_ENEMY,0),5);
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",255,230,TYPE_ENEMY,0),5);

    SetSpawnPoint(25,406)
    
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
    ChangeMap("assets/encounters/01/map_4.lua")
    return false;
end