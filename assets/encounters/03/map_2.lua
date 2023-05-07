

function setup()
    SetMapSprite("assets/encounters/03/map_2.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/enemies/prowler/prowler.lua",197.00,45.00,TYPE_ENEMY,5);

    SetSpawnPoint(134,719);
    CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",191.00,207.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",149.00,199.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",184.00,246.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",142.00,465.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",103.00,467.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",83.00,598.00,TYPE_ENEMY,0.00),0)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",112.00,328.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",73.00,343.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",185.00,331.00,TYPE_ENEMY,0.00),2)
end

function update(dt)
end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/03/map_final.lua")
    return false;
end