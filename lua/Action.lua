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
      obj.queue = function(act) master:queue(act) end
      return master
   end
   setmetatable(c, mt)
   return c
end
