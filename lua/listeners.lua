require 'WalkLoop'

function printMap()
   local map = data.getCurrentMap()
   for y = 0, map:getHeight() - 1 do
      for x = 0, map:getWidth() - 1 do
         io.write(string.format("%02x ", map:getNode(x, y):getStatus()))
      end
      io.write("\n")
   end
end

function onRefresh()
   local p = data.player
   local map = data.getCurrentMap()
   print("Player coordinates: (" .. p:getX() .. ", " .. p:getY() .. ")")
   print("Map [" .. map:getName() .. "]: " .. map:getWidth() .. "x" .. map:getHeight())
end

function onInit()
   config.setNumber("clearOnRefresh", 1)
   bot.queue(new.lua(WalkLoop(1)))
   bot.queue(new.talkTo(0))
end
