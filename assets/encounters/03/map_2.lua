

function setup()
    SetMapSprite("assets/encounters/03/map_2.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/enemies/prowler/prowler.lua",197.00,32.00,TYPE_ENEMY,5);

    SetSpawnPoint(134,798);
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",160.00,220.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",131.00,215.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",156.00,276.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",136.00,535.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",96.00,539.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",72.00,667.00,TYPE_ENEMY,0.00),0)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",128.00,372.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",93.00,392.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",186.00,389.00,TYPE_ENEMY,0.00),2)
end

function update(dt)
end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/03/map_3.lua")
    return false;
end