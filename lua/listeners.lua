require 'WalkLoop'

function printMap()
   local p = data.player
   local map = data.world:getMap(p:getBank(), p:getMap())
   for y = 0, map:getHeight() - 1 do
      for x = 0, map:getWidth() - 1 do
         io.write(string.format("%02x ", map:getNode(x, y):getStatus()))
      end
      io.write("\n")
   end
end

function onEnterFrame()
   local p = data.player
   local map = data.world:getMap(p:getBank(), p:getMap())
   print("Player coordinates: (" .. p:getX() .. ", " .. p:getY() .. ")")
   print("Map [" .. map:getName() .. "]: " .. map:getWidth() .. "x" .. map:getHeight())
end

function onInit()
   bot.queue(new.lua(WalkLoop(1)))
   bot.queue(new.talkTo(0))
end
