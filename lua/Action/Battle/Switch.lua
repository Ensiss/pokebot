-- Switch to the specified pokemon from the ones not already in battle, numbered from 0
function battle.switch(poke)
   local bm = pb.getBattleMenu()

   -- TODO: check if the index is valid and the pokemon there has not fainted
   if bm:getMenu() ~= 0 then return -1 end

   moveCursor(2, 2, 2, function() return bm:getCursor() end)
   while bm:getMenu() == 0 do
      pressButton(btn.a)
   end
   for i = 1, 75 do coroutine.yield() end
   pressButton(btn.right)
   for i = 0, poke - 1 do pressButton(btn.down) end
   for i = 0, 1 do pressButton(btn.a) end
end