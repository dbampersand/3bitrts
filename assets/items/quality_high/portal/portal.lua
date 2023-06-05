local hasMadeUnit = false
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
    SetItemName(obj,"Portal")
    SetItemDescription(obj,"Permanently summons an extra hero unit.")
    SetItemGoldCost(obj,100)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/portal/portal.png");
end

function attached(obj)
end

function update(dt)
    SetItemHighlight(item,false)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end

function ondamaged(item,sourceObj,damagedObj,value)
end
function onmapchange(obj)
    if (hasMadeUnit == false) then
        local summoned = RandRangeI (1,#summonable)
        local summ = CreateObject(summonable[summoned],GetX(GetObjRef()),GetY(GetObjRef()),TYPE_FRIENDLY,1);
        hasMadeUnit = true
    end
end