

function setup()
    SetMapSprite("assets/encounters/03/map_2.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/enemies/prowler/prowler.lua",196.00,95.00,TYPE_ENEMY,5);

    SetSpawnPoint(134,859);
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",158.00,277.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",130.00,273.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",156.00,335.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",134.00,593.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",94.00,597.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",70.00,725.00,TYPE_ENEMY,0.00),0)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",126.00,429.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",90.00,449.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",184.00,447.00,TYPE_ENEMY,0.00),2)
    PlayMusic("assets/audio/music/encounters/cusith/cusith_encounter_1.wav",0.5,0); 
end

function update(dt)
end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/03/map_3.lua")
    return false;
end