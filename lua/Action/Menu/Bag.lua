menu.bag = {}

function menu.bag.open()
  if pb.getBagMenu():isOpen() then return 0 end

  local start = pb.getStartMenu()
  menu.open()
  misc.moveCursor(1, start:getNbItems(), menu.getItemPosition(MENUITEM.BAG), function() return start:getCursor() end)
  misc.wait(10)
  misc.pressButton(btn.a)
  misc.wait(75)
end

function menu.bag.selectItem(itemId)
  if not BagUtils.hasItem(itemId) then return -1 end

  local bag = pb.getBag()
  local bagMenu = pb.getBagMenu()
  local item = pb.getItem(itemId)
  local pocketLoc, itemLoc = BagUtils.getLocation(itemId)
  local pocket = bag:getPocket(pocketLoc)

  menu.bag.open()

  if pocketLoc < 3 then
    -- Changing back pocket keeps your input but doesn't change the values immediately
    -- This prevents the use of misc.moveCursor because it spams the keys until the value changes
    local diff = pocketLoc - bagMenu:getPocket()
    for i = 0, math.abs(diff) - 1 do
      misc.pressButton(diff > 0 and btn.right or btn.left)
    end
  else
    menu.bag.selectItem(pocketLoc == pocketType.TMCase and ITEM.TMCASE or ITEM.BERRYPOUCH)
    misc.pressButton(btn.a)
    misc.wait(75)
  end

  -- Move to the right item
  misc.moveCursor(1, pocket:getCapacity(), itemLoc, function() return bagMenu:getCursor() + bagMenu:getScroll() end)
  misc.wait(50)
  misc.pressButton(btn.a)
  misc.wait(50)
end

function menu.bag.use(itemId, target)
  -- TODO: Check that the item can be used
  menu.bag.selectItem(itemId)
  misc.pressButton(btn.a)
  -- TODO: Select the pokemon to use the item on
  -- TODO: Exit menus
end
