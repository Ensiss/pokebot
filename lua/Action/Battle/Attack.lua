-- Attacks using the specified attack, starting at index 0
function battle.attack(atk)
   local bm = pb.getBattleMenu()

   -- TODO: check if the attack is unlocked and has enough PPs
   if bm:getMenu() ~= 0 then return -1 end

   moveCursor(2, 2, 0, function() return bm:getCursor() end)
   while bm:getMenu() == 0 do
      pressButton(btn.a)
   end
   moveCursor(2, 2, atk, function() return bm:getAttack() end)
   while bm:isOpen() == 1 do
      pressButton(btn.a)
   end
end
