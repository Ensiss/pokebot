require('Action/Misc')

-- Attempt to flee the battle
local function _run()
   local bm = pb.getBattleMenu()

   moveCursor(2, 2, 3, function() return bm:getCursor() end)
   while bm:getMenu() == 0 do
      pressButton(btn.a)
   end
end

-- Switch to the specified pokemon from the ones not already in battle, numbered from 0
local function _switch(poke)
   local bm = pb.getBattleMenu()

   moveCursor(2, 2, 2, function() return bm:getCursor() end)
   while bm:getMenu() == 0 do
      pressButton(btn.a)
   end
   for i = 1, 75 do coroutine.yield() end
   pressButton(btn.right)
   for i = 0, poke - 1 do pressButton(btn.down) end
   for i = 0, 1 do pressButton(btn.a) end
end

-- Attacks using the specified attack, starting at index 0
local function _attack(atk)
   local bm = pb.getBattleMenu()

   moveCursor(2, 2, 0, function() return bm:getCursor() end)
   while bm:getMenu() == 0 do
      pressButton(btn.a)
   end
   moveCursor(2, 2, atk, function() return bm:getAttack() end)
   while bm:isOpen() == 1 do
      pressButton(btn.a)
   end
end

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

function naiveBattleAI()
   local bm = pb.getBattleMenu()

   repeat
      if bm:isOpen() ~= 0 and bm:getMenu() == 0 then
         _attack(_getBestMove())
      else
         pressButton(btn.a)
      end
   until false
   return 0
end
