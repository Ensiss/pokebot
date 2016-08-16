function menu.open()
  if pb.isInBattle() then return -1 end
  if pb.getStartMenu():isOpen() then return 0 end
  misc.pressButton(btn.start)
  misc.waitUntil(function()
      local start = pb.getStartMenu()
      return start:isOpen() and start:getState() == 5
  end)
  misc.wait(10)
  return 0
end

function menu.close()
  if pb.isInBattle() then return -1 end
  if not pb.getStartMenu():isOpen() then return 0 end
  misc.pressButton(btn.start)
  misc.waitUntil(function() return not pb.getStartMenu():isOpen() end)
  return 0
end

function menu.getItemPosition(item)
  local start = pb.getStartMenu()
  for i = 0, start:getNbItems() - 1 do
    if start:getItemIndex(i) == item then return i end
  end
  return -1
end

MENUITEM = {
  POKEDEX = 0,
  POKEMON = 1,
  BAG = 2,
  TRAINER = 3,
  SAVE = 4,
  OPTION = 5,
  EXIT = 6,
  RETIRE = 7,
  TRAINER2 = 8
}
