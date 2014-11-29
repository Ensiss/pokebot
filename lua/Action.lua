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
      return obj
   end
   setmetatable(c, mt)
   return c
end
