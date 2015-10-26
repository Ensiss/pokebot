local function _searchOW(personId)
   local p = pb.getPlayer()
   local m = pb.getCurrentMap()
   local pers = m:getPerson(personId)
   local pbank, pmap = p:getBankId(), p:getMapId()

   for i = 1, 15 do
      local ow = pb.getOverWorld(i)
      local owmap, owbank = ow:getMapId(), ow:getBankId()

      if owmap == 0 and owbank == 0 then break end
      if owmap == pmap and owbank == pbank and ow:getEventNb() == pers:getEventNb() then
         return ow
      end
   end
   return nil
end

local function _turnToOW(personId)
   local dir2key = {btn.down, btn.up, btn.left, btn.right}
   local p = pb.getPlayer()
   local m = pb.getCurrentMap()
   local pers = m:getPerson(personId)
   local tx, ty = pers:getX(), pers:getY()
   local px, py = p:getX(), p:getY()
   local dir

   local ow = _searchOW(personId)
   if ow then
      tx = ow:getDestX()
      ty = ow:getDestY()
   end
   if py < ty then dir = 1
   elseif py > ty then dir = 2
   elseif px > tx then dir = 3
   else dir = 4 end
   if pb.getOverWorld(0):getDir() ~= dir then
      misc.pressButton(dir2key[dir])
   end
end

local function _searchCmd(pscript, nextCmd)
   local curr = pscript:getInstruction(pscript:getStartOffset())

   while curr:getNext() ~= 0 do
      if curr:getNext() == nextCmd then return curr end
      curr = pscript:getInstruction(curr:getNext())
   end
   return nil
end

local function _getCurrentCommand(pscript)
   local nextCmd = mem.get32(0x03000EB8);
   local instr = _searchCmd(pscript, nextCmd)

   if instr ~= nil then return instr end
   for i = 0, 9 do
      instr = _searchCmd(script.getStd(i), nextCmd)
      if instr ~= nil then return instr end
   end
   return nil
end

function interact.talkTo(personId)
   local p = pb.getPlayer()
   local m = pb.getCurrentMap()

   if personId < 0 or personId >= m:getNbPersons() then
      print("talkTo error: invalid person id %d in map [%d, %d]", personId, p:getBankId(), p:getMapId())
      return -1
   end

   local pers = m:getPerson(personId)
   local pscript = script.getPerson(personId, 0, 0)
   local tx, ty = pers:getX(), pers:getY()
   local oldnext = 0

   repeat
      local ow = _searchOW(personId)
      if ow then
         tx, ty = ow:getDestX(), ow:getDestY()
      end

      if move.npc(personId) == -1 then return -1 end
      _turnToOW(personId)
      misc.pressButton(btn.a)
      misc.pressButton(btn.a)
   until math.abs(tx - p:getX()) + math.abs(ty - p:getY()) == 1 and _getCurrentCommand(pscript)

   -- if the talking scripts are finished, stop the loop
   while mem.get8(0x03000F9C) ~= 0 do
      local instr = _getCurrentCommand(pscript)
      local framePassed = false
      if instr ~= nil then
         local cmd = instr:getCommand()
         if cmd == 0x66 then                       -- waitmsg
            local ptr = mem.get32(0x02020034)
            local chr = mem.get8(ptr - 1)          -- last character displayed
            if chr == 0xFA or chr == 0xFB then
               misc.pressButton(btn.a)
               framePassed = true
            end
         elseif cmd == 0x6D or cmd == 0x68 then    -- waitkeypress/closeonkeypress
            if oldnext ~= instr:getNext() then
               misc.pressButton(btn.a)
               framePassed = true
               oldnext = instr:getNext()
            end
         -- elseif cmd == 0x6F then                   -- multichoice
         --    _handleMultiChoice()
         -- elseif cmd == 0x6E then                   -- yesnobox
         --    _handleYesNo()
         end
      end
      if not framePassed then misc.nextFrame() end
   end
   return 0
end
