WalkLoop = setmetatable({}, {
      __index = WalkLoop,
      __call = function(act, radius)
         local action = {}
         setmetatable(action, {__index = WalkLoop})
         action.radius = radius
         return action
      end
})

function WalkLoop:init(master)
   local x = data.player:getX()
   local y = data.player:getY()
   master:queue(new.moveTo(x - 1, y - self.radius))
   master:queue(new.moveTo(x - self.radius, y - self.radius))
   master:queue(new.moveTo(x - self.radius, y))
   master:queue(new.moveTo(x, y))
end

function WalkLoop:update(master)
   return state.finished
end
