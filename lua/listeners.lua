require 'Action/Movement'
require 'Action/Battle'
require 'Action/Misc'
require 'Wrapper'

function printMap()
   local map = pb.getCurrentMap()
   for y = 0, map:getHeight() - 1 do
      for x = 0, map:getWidth() - 1 do
         io.write(string.format("%02x ", map:getNode(x, y):getStatus()))
      end
      io.write("\n")
   end
end

function onRefresh()
   local p = pb.getPlayer()
   local map = pb.getCurrentMap()
   print("Player coordinates: (" .. p:getX() .. ", " .. p:getY() .. ")")
   print("Map [" .. map:getName() .. "]: " .. map:getWidth() .. "x" .. map:getHeight())
end

function onInit()
   local bot = pb.getBot()

   config.setNumber("clearOnRefresh", 1)

   -- Main script for the bot, called every frame to continue execution
   local botScript = wrap(function()
         moveTo(3, 3)
         moveTo(0)
         moveTo(3, 3)
         useConnection(connect.up)
         repeat
            -- moveLoop(btn.left)
            if moveLoop(btn.up) == -1 then coroutine.yield() end
            -- moveLoop(btn.right)
            -- moveLoop(btn.down)
         until false
   end, naiveBattleAI)

   bot:queue(botScript)
end
