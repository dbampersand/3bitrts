function setup()

    SetSpawnPoint(109,631)
    SetMapSprite("assets/encounters/03/map_7.png")
    CreateObject("assets/enemies/apprentice_lich/apprentice_lich.lua",162.00,495.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/apprentice_lich/apprentice_lich.lua",206.00,314.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/apprentice_lich/apprentice_lich.lua",170.00,326.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/apprentice_lich/apprentice_lich.lua",298.00,146.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/apprentice_lich/apprentice_lich.lua",246.00,126.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",280.00,198.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",232.00,172.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",442.00,162.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",442.00,138.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",554.00,302.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/apprentice_lich/apprentice_lich.lua",566.00,374.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/apprentice_lich/apprentice_lich.lua",498.00,374.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",416.00,146.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",502.00,311.00,TYPE_ENEMY,0.00),5)
end
function update()

end
function mapend()

end