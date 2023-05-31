local timer = 0

function setup(obj)
    SetItemName(obj,"Mana Stone")
    SetItemDescription(obj,"Increases your total mana and mana regeneration rate.")
    SetItemGoldCost(obj,20)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/stone_feather/stone_feather.png");
end

function attached(obj)
    
end

function update(dt)
end

function OnAttack(item,obj,target,dt,value)
end

function OnEffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
