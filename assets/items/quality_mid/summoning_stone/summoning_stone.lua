local summonable = {
    "assets/friendly/necromancer/necromancer.lua",
    "assets/friendly/bard/bard.lua",
    "assets/friendly/shaman/shaman.lua",
    "assets/friendly/arcane_mage/arcane_mage.lua",
    "assets/friendly/chromamancer/chromamancer.lua",
    "assets/friendly/warrior/warrior.lua",
    "assets/friendly/herbalist/herbalist.lua",
    "assets/friendly/rogue/rogue.lua",
    "assets/friendly/rune_smith/rune_smith.lua",
    "assets/friendly/ranger/ranger.lua",
    "assets/friendly/paladin/paladin.lua",
    "assets/friendly/monk/monk.lua",
    "assets/friendly/priest/priest.lua"
}

function setup(obj)
    SetItemName(obj,"Summoning Stone")
    SetItemDescription(obj,"Summon a random friendly unit that leaves when descending.")
    SetItemGoldCost(obj,90)
    SetItemTier(obj,1)
    SetItemIcon(obj, "assets/items/quality_mid/summoning_stone/summoning_stone.png");

end

function attached(obj)

end

function unattach(obj)
end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function onmapchange(obj)
    local summoned = RandRangeI (1,#summonable)
    local summ = CreateObject(summonable[summoned],GetX(GetObjRef()),GetY(GetObjRef()),TYPE_FRIENDLY,1);
    SetObjSummoned(summ,true)
end