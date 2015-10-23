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
         for i = 1, 3 do
            move.turn(btn.right)
            move.turn(btn.down)
            move.turn(btn.left)
            move.turn(btn.up)
         end

         move.warp(0)
         move.to(3, 3)
         move.warp(1)

         move.to(3, 3)
         move.to(0)
         move.to(3, 3)
         move.connection(connect.up)
         repeat
            if move.loop(btn.up) == -1 then coroutine.yield() end
         until false
   end, naiveBattleAI)

   bot:queue(botScript)
end
