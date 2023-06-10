
function setup()

    SetMapSprite("assets/encounters/0/map_1.png")



    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",300.00,403.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",320.00,403.00,TYPE_ENEMY,0),1);



    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",296.00,275.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",344.00,279.00,TYPE_ENEMY,0),2);



    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",252.00,143.00,TYPE_ENEMY,0),3);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",342.00,180.00,TYPE_ENEMY,0),3);
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",301.00,208.00,TYPE_ENEMY,0),3);



 

    SetSpawnPoint(295,542)

    PlayMusic("assets/audio/music/encounters/kobold/kobold_encounter.wav",0.5,12); 

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",219.00,191.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",116.00,171.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",148.00,87.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",100.00,107.00,TYPE_ENEMY,0.00),4)
end



function update(dt)

end



function kill()



end



function mapend()
    --GoShop(true);

    ChangeMap("assets/encounters/0/map_2.lua")
    return false;

end