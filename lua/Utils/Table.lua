-- Returns the min index and value in a given table
function table.min(t, getValue)
   getValue = getValue or function(x) return x end

   local mini = 0
   local minval = math.huge

   for i, val in pairs(t) do
      if mini == 0 or getValue(val) < getValue(minval) then
         mini = i
         minval = val
      end
   end
   return mini, minval
end

-- Returns the max index and value in a given table
function table.max(t, getValue)
   getValue = getValue or function(x) return x end

   local maxi = 0
   local maxval = -math.huge

   for i, val in ipairs(t) do
      if maxi == 0 or getValue(val) > getValue(maxval) then
         maxi = i
         maxval = val
      end
   end
   return maxi, maxval
end

-- Searches for the first occurrence of an element in a table
function table.find(t, tofind, getValue)
   getValue = getValue or function(x) return x end

   tofind = getValue(tofind)
   for i, val in ipairs(t) do
      if getValue(val) == tofind then
         return i
      end
   end
   return nil
end
