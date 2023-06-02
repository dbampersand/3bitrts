local maxHP = 200

function setup(obj)
    SetItemName(obj,"Breastplate")
    SetItemDescription(obj,"Increases maximum health by "..maxHP)
    SetItemGoldCost(obj,30)
    SetItemTier(obj,0)
    SetItemIcon(obj, "assets/items/quality_poor/breastplate/breastplate.png");

    
end

function attached(obj)      
    SetMaxHP(GetMaxHP()+maxHP,true)
    

end
function unattach(obj)
    SetMaxHP(GetMaxHP()-maxHP,false)
end


function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)
end
