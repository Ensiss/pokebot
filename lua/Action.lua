function ActionClass(init)
   local c = {}
   c.__index = c
   local mt = {}
   mt.__call = function(class, ...)
      local obj = {}
      setmetatable(obj, c)
      if init then
         init(obj, ...)
      end
      local master = new.lua(obj)
      obj.master = master
      obj.queue = function(self, act) return master:queue(act) end
      obj.getId = function(self) return master:getId() end
      obj.getState = function(self) return master:getState() end
      obj.getCounter = function(self) return master:getCounter() end
      obj.getFinishedChild = function(self) return master:getFinishedChild() end
      obj.addListener = function(self, signal, func) master:addListener(signal, func) end
      return master
   end
   setmetatable(c, mt)
   return c
end
