

function setup()
    SetMapSprite("assets/encounters/03/map_3.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);

    SetSpawnPoint(289,382);
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",436.00,385.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",440.00,353.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",432.00,425.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",462.00,187.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",434.00,227.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",486.00,627.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",446.00,607.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",324.00,193.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",336.00,153.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",240.00,189.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",248.00,137.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",228.00,161.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",168.00,209.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",140.00,189.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",110.00,279.00,TYPE_ENEMY,0.00),7)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",86.00,259.00,TYPE_ENEMY,0.00),7)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",40.00,389.00,TYPE_ENEMY,0.00),8)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",52.00,333.00,TYPE_ENEMY,0.00),8)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",44.00,409.00,TYPE_ENEMY,0.00),8)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",44.00,365.00,TYPE_ENEMY,0.00),8)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",102.00,467.00,TYPE_ENEMY,0.00),9)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",140.00,445.00,TYPE_ENEMY,0.00),9)
    SetAggroGroup(CreateObject("assets/enemies/wolf/wolf.lua",96.00,601.00,TYPE_ENEMY,0.00),10)
    SetAggroGroup(CreateObject("assets/enemies/wolf/wolf.lua",88.00,581.00,TYPE_ENEMY,0.00),10)
    SetAggroGroup(CreateObject("assets/enemies/wolf/wolf.lua",108.00,625.00,TYPE_ENEMY,0.00),10)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",206.00,627.00,TYPE_ENEMY,0.00),11)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",230.00,587.00,TYPE_ENEMY,0.00),11)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",308.00,649.00,TYPE_ENEMY,0.00),12)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",344.00,669.00,TYPE_ENEMY,0.00),12)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",316.00,685.00,TYPE_ENEMY,0.00),12)
end

function update(dt)
end

function kill()

end

function mapend()
    GoShop(true)
    ChangeMap("assets/encounters/03/map_4.lua")
    return false;
end