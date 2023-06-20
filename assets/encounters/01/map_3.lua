
function setup()
    SetMapSprite("assets/encounters/01/map_3.png")

    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",50.00,105.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",88.00,82.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",77.00,116.00,TYPE_ENEMY,0),1);
    
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",89,294,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",96,301,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",136.00,314.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",164.00,339.00,TYPE_ENEMY,0),2);

    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",222.00,225.00,TYPE_ENEMY,0),3);
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",270,311,TYPE_ENEMY,0),3);

  
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",312,305,TYPE_ENEMY,0),4);
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",330,320,TYPE_ENEMY,0),4);

    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",208.00,195.00,TYPE_ENEMY,0),5);
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",203.00,216.00,TYPE_ENEMY,0),5);

    SetSpawnPoint(25,406)
    
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",237.00,203.00,TYPE_ENEMY,0.00),3)
    --PlayMusic("assets/audio/music/encounters/wyrm/wyrm_encounter_1.wav",0.5,16.36); 

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
    GoShop(true)
    ChangeMap("assets/encounters/01/map_4.lua")
    return false;
end