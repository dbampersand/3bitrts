

function setup()
    SetMapSprite("assets/encounters/03/map_3.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);

    SetSpawnPoint(289,281);
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",438.00,287.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",443.00,253.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",434.00,326.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",465.00,89.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",435.00,128.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",486.00,526.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",446.00,506.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",325.00,92.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",336.00,52.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",246.00,107.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",232.00,69.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",224.00,93.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",180.00,139.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",165.00,113.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",110.00,181.00,TYPE_ENEMY,0.00),7)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",88.00,157.00,TYPE_ENEMY,0.00),7)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",42.00,290.00,TYPE_ENEMY,0.00),8)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",50.00,235.00,TYPE_ENEMY,0.00),8)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",45.00,309.00,TYPE_ENEMY,0.00),8)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",46.00,266.00,TYPE_ENEMY,0.00),8)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",104.00,367.00,TYPE_ENEMY,0.00),9)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",142.00,344.00,TYPE_ENEMY,0.00),9)
    SetAggroGroup(CreateObject("assets/enemies/wolf/wolf.lua",96.00,503.00,TYPE_ENEMY,0.00),10)
    SetAggroGroup(CreateObject("assets/enemies/wolf/wolf.lua",91.00,481.00,TYPE_ENEMY,0.00),10)
    SetAggroGroup(CreateObject("assets/enemies/wolf/wolf.lua",111.00,526.00,TYPE_ENEMY,0.00),10)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",209.00,526.00,TYPE_ENEMY,0.00),11)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",232.00,489.00,TYPE_ENEMY,0.00),11)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",311.00,549.00,TYPE_ENEMY,0.00),12)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",344.00,568.00,TYPE_ENEMY,0.00),12)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",319.00,586.00,TYPE_ENEMY,0.00),12)
end

function update(dt)
end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/03/map_4.lua")
    return false;
end