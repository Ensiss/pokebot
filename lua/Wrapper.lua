local function saveKeyState(keyState)
   for i = 0, btn.count - 1 do
      keyState[i] = pb.getButton(i)
   end
end

local function loadKeyState(keyState)
   for i = 0, btn.count - 1 do
      pb.setButton(i, keyState[i])
   end
end

-- Creates a function callable by the bot
-- It will resume the coroutine when called
-- Returns -1 when there's an error or the coroutine is dead
-- Returns 0 otherwise
function wrap(script, battleScript)
   local keysState = {}
   local wasInBattle = false
   local co = coroutine.create(script)
   local coBattle = nil
   if battleScript then
      coBattle = coroutine.create(battleScript)
   end

   return function()
      local currCo
      if pb.isInBattle() then
         currCo = coBattle
      else
         currCo = co
      end

      if pb.isInBattle() ~= wasInBattle then
         if wasInBattle then
            loadKeyState(keysState)
         else
            saveKeyState(keysState)
         end
      end

      wasInBattle = pb.isInBattle()

      if currCo then
         local code, msg = coroutine.resume(currCo)

         if code == false or coroutine.status(currCo) == 'dead' then
            if code == false then
               print("Lua error:", msg)
            end
            return -1
         end
      else
         print("No coroutine to resume")
      end
      return 0
   end
end
