package.path = "lua/?.lua;" .. package.path
require 'Action/Misc'
require 'Action/Battle'
require 'Action/Movement'
require 'Action/Interaction'
require 'Utils/Wrapper'

require 'Custom/World/SimpleWorldAI'
require 'Custom/Battle/SimpleBattleAI'

function printMap()
   local map = pb.getCurrentMap()
   for y = 0, map:getHeight() - 1 do
      for x = 0, map:getWidth() - 1 do
         io.write(string.format("%02x ", map:getNode(x, y):getStatus()))
      end
      io.write("\n")
   end
end

function exportMap(file, bankId, mapId, rec, xoffset, yoffset, visitedScreens)
  rec = rec or 2000
  xoffset = xoffset or 0
  yoffset = yoffset or 0
  visitedScreens = visitedScreens or {}

  local screenId = bankId * 1000 + mapId
  if table.find(visitedScreens, screenId) ~= nil then return end
  table.insert(visitedScreens, screenId)

  local map = pb.getMap(bankId, mapId)

  function mkId(x, y, bId, mId)
    bId = bId or bankId
    mId = mId or mapId
    local nodestr = string.format("node_b%d_m%d", bId, mId)
    return string.format("%s_x%d_y%d", nodestr, x, y)
  end

  function mkEdge(node1, node2)
    return string.format('\t%s -> %s\n', node1, node2)
  end

  function mkDoubleEdge(node1, node2)
    return string.format('\t%s -> %s -> %s\n', node1, node2, node1)
  end

  function isWalkable(node)
    local _walkableTiles = {0x0c, 0x00, 0x10, 0x04}
    return table.find(_walkableTiles, node:getStatus()) ~= nil
  end

  -- Export map insides
  for y = 0, map:getHeight() - 1 do
    for x = 0, map:getWidth() - 1 do
      if isWalkable(map:getNode(x, y)) then
        local color = "brown"
        if map:getNode(x, y):getBehavior() == 0x0202 then color = "green"
        elseif map:getNode(x, y):getStatus() == 0x04 then color = "blue" end
        file:write(string.format('\t%s[shape=circle, style=filled, fillcolor=%s, label="", pos = "%d, %d!"];\n',
                                 mkId(x, y), color, x + xoffset, -y - yoffset))

        if x < map:getWidth() - 1 then
          if isWalkable(map:getNode(x + 1, y)) then -- Right node
            file:write(mkDoubleEdge(mkId(x, y), mkId(x + 1, y)))
          elseif map:getNode(x + 1, y):getBehavior() == 0x38 and map:getNode(x + 2, y):getStatus() == 0x0C then -- jumpable right hill
            file:write(mkEdge(mkId(x, y), mkId(x + 2, y)))
          elseif map:getNode(x + 1, y):getBehavior() == 0x39 and map:getNode(x + 2, y):getStatus() == 0x0C then -- Jumpable left hill
            file:write(mkEdge(mkId(x + 2, y), mkId(x, y)))
          end
        end
        if y < map:getHeight() - 1 then
          if isWalkable(map:getNode(x, y + 1)) then -- Down node
            file:write(mkDoubleEdge(mkId(x, y), mkId(x, y + 1)))
          elseif map:getNode(x, y + 1):getBehavior() == 0x3B and map:getNode(x, y + 2):getStatus() == 0x0C then -- Jumpable down hill
            file:write(mkEdge(mkId(x, y), mkId(x, y + 2)))
          end
        end
      end
    end
  end

  -- Export borders
  for i = 0, map:getNbConnections() - 1 do
    repeat
      local connection = map:getConnection(i)
      local dir = connection:getType()

      -- Dive and emerge not supported
      if dir == connect.dive or dir == connect.emerge then break end

      local vtcl = (dir == connect.down or dir == connect.up)
      local xstep = vtcl and 1 or 0
      local ystep = math.mod(xstep + 1, 2)
      local x = dir == connect.right and map:getWidth() - 1 or 0
      local y = dir == connect.down and map:getHeight() - 1 or 0
      local dBankId = connection:getBankId()
      local dMapId = connection:getMapId()
      local dmap = pb.getMap(dBankId, dMapId)
      local offset = connection:getOffset()
      local linkFound = false

      while x < map:getWidth() and y < map:getHeight() do
        -- compute index in the destination map
        local dx
        local dy

        if vtcl then
          dx = x - connection:getOffset()
          dy = dir == connect.up and dmap:getHeight() - 1 or 0
        else
          dy = y - connection:getOffset()
          dx = dir == connect.left and dmap:getWidth() - 1 or 0
        end

        if dx >= 0 and dx < dmap:getWidth() and
          dy >= 0 and dy < dmap:getHeight() and -- check out of bounds
          isWalkable(map:getNode(x, y)) and -- walkable tile in the current map
          isWalkable(dmap:getNode(dx, dy)) -- walkable in the destination map
        then
          if rec > 0 and not linkFound then
            local mapw = dir == connect.right and map:getWidth() or -dmap:getWidth()
            local maph = dir == connect.down and map:getHeight() or -dmap:getHeight()
            local dxoff = xoffset + xstep * offset + math.mod(xstep + 1, 2) * mapw
            local dyoff = yoffset + ystep * offset + math.mod(ystep + 1, 2) * maph
            exportMap(file, dBankId, dMapId, rec - 1, dxoff, dyoff, visitedScreens)
            linkFound = true
          end
          if rec > 0 and (dir == connect.down or dir == connect.right) then
            file:write(mkDoubleEdge(mkId(x, y, bankId, mapId), mkId(dx, dy, dBankId, dMapId)))
          end
        end

        x = x + xstep
        y = y + ystep
      end
    until true
  end
end

function exportWorld()
  local player = pb.getPlayer()
  local file = assert(io.open("map.dot", "w"))
  file:write("digraph map {\n")

  exportMap(file, 3, 1)

  file:write("}\n")
  file:close()
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
   -- You need an AI for the outside world and another to handle battles
   local botScript = wrap(SimpleWorldAI, SimpleBattleAI)
   bot:queue(botScript)
end
