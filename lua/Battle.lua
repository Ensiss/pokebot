require 'Action'

Battle = ActionClass()

function Battle:init()
end

function Battle:update()
   bm = pb.getBattleMenu()
   if bm:isOpen() ~= 0 and bm:getMenu() == 0 then
      self:_attack(self:_getBestMove())
   else
      self:queue(new.pressButton(btn.a))
   end
end

function Battle:_attack(atk)
   self:queue(new.moveCursor(2, 2, 0, function() return pb.getBattleMenu():getCursor() end))
   self:queue(new.pressButton(btn.a))
   self:queue(new.moveCursor(2, 2, atk, function() return pb.getBattleMenu():getAttack() end))
   self:queue(new.pressButton(btn.a))
end

function Battle:_switch(poke)
   self:queue(new.moveCursor(2, 2, 2, function() return pb.getBattleMenu:getCursor() end))
   self:queue(new.pressButton(btn.a))
   self:queue(new.wait(75))
   self:queue(new.pressButton(btn.right))
   for i = 0, poke - 1 do self:queue(new.pressButton(btn.down)) end
   for i = 0, 1 do self:queue(new.pressButton(btn.a)) end
end

function Battle:_run()
   self:queue(new.moveCursor(2, 2, 3, function() return pb.getBattleMenu:getCursor() end))
   self:queue(new.pressButton(btn.a))
end

function Battle:_getBestMove()
   local p = pb.getBattler(0)
   local e = pb.getBattler(1)
   local best = 0
   local min = 0

   for m = 0, 3 do
      if p:getMoveId(m) ~= 0 and p:getPP(m) > 0 then
         local move = pb.getMove(p:getMoveId(m))
         local dmg = pb.getDamage(p, e, move)

         if (min < e:getHP() and dmg:getMin() > min) or
            (dmg:getMin() > e:getHP() and dmg:getMin() < min) then
               best = m
               min = dmg:getMin()
         end
      end
   end
   print("Best: " .. pb.getMove(p:getMoveId(best)):getName())
   return best
end
