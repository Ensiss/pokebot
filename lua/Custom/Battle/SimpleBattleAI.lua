local function _getBestMove()
   local p = pb.getBattler(0)
   local e = pb.getBattler(1)
   local best = 0
   local min = 0

   for m = 0, 3 do
      if p:getMoveId(m) ~= 0 and p:getPP(m) > 0 then
         local move = pb.getMove(p:getMoveId(m))
         local dmg = pb.getDamage(p, e, move)

         if (min < e:getHP() and dmg:getMin() > min) or
            (dmg:getMin() > e:getHP() and dmg:getMin() < min)
         then
               best = m
               min = dmg:getMin()
         end
      end
   end
   print("Best: " .. pb.getMove(p:getMoveId(best)):getName())
   return best
end

function SimpleBattleAI()
   local bm = pb.getBattleMenu()

   repeat
      if bm:isOpen() ~= 0 and bm:getMenu() == 0 then
         battle.attack(_getBestMove())
      else
         pressButton(btn.a)
      end
   until false
   return 0
end
