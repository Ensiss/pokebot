-- Attempt to flee the battle
function battle.run()
   local bm = pb.getBattleMenu()

   if bm:getMenu() ~= 0 then return -1 end
   misc.moveCursor(2, 2, 3, function() return bm:getCursor() end)
   while bm:getMenu() == 0 do
      misc.pressButton(btn.a)
   end
   return 0
end

function battle.flee()
   return battle.run()
end
