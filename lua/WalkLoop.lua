require 'Action'

WalkLoop = ActionClass(function(obj, radius)
      obj.radius = radius
end)

function WalkLoop:init()
   local x = data.player:getX()
   local y = data.player:getY()
   self.queue(new.moveTo(x - 1, y - self.radius))
   self.queue(new.moveTo(x - self.radius, y - self.radius))
   self.queue(new.moveTo(x - self.radius, y))
   self.queue(new.moveTo(x, y))
end

function WalkLoop:update()
   return state.finished
end
