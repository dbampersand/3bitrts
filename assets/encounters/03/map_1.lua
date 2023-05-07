

function setup()
    SetMapSprite("assets/encounters/03/map_1.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);

    SetSpawnPoint(108,646);
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",85.00,500.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",141.00,487.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",146.00,371.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",103.00,213.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",160.00,216.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",184.00,104.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",132.00,104.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",164.00,70.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",124.00,258.00,TYPE_ENEMY,0.00),3)
end

function update(dt)
end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/03/map_2.lua")
    return false;
end