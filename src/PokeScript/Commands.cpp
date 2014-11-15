#include        "Script.hh"

Script::Command Script::_cmds[0xD6] = {
  /* 00 */ Command("nop", ""),
  /* 01 */ Command("nop1", ""),
  /* 02 */ Command("end", ""),
  /* 03 */ Command("return", ""),
  /* 04 */ Command("call 0x%08x", "ptr", &Script::_branch),
  /* 05 */ Command("goto 0x%08x", "ptr", &Script::_branch),
  /* 06 */ Command("if1 %#x 0x%08x", "byte ptr", &Script::_if),
  /* 07 */ Command("if2 %#x 0x%08x", "byte ptr", &Script::_if),
  /* 08 */ Command("gotostd %#x", "byte"),
  /* 09 */ Command("callstd %#x", "byte"),
  /* 0A */ Command("gotostdif %#x %#x", "byte byte"),
  /* 0B */ Command("callstdif %#x %#x", "byte byte"),
  /* 0C */ Command("jumpram", ""),
  /* 0D */ Command("killscript", ""),
  /* 0E */ Command("setbyte %#x", "byte"),
  /* 0F */ Command("loadpointer %d %#x", "bank dword", &Script::_loadpointer),
  /* 10 */ Command("setbyte2 %d %#x", "bank byte"),
  /* 11 */ Command("writebytetooffset %#x 0x%08x", "byte ptr"),
  /* 12 */ Command("loadbytefrompointer %d 0x%08x", "bank ptr"),
  /* 13 */ Command("setfarbyte %d 0x%08x", "bank ptr"),
  /* 14 */ Command("copyscriptbanks %d %d", "bank bank"),
  /* 15 */ Command("copybyte 0x%08x 0x%08x", "ptr ptr"),
  /* 16 */ Command("setvar %#x %#x", "var word"),
  /* 17 */ Command("addvar %#x %#x", "var word"),
  /* 18 */ Command("subvar %#x %#x", "var word/var"),
  /* 19 */ Command("copyvar %#x %#x", "var var"),
  /* 1A */ Command("copyvarifnotzero %#x %#x", "var word/var"),
  /* 1B */ Command("comparebanks %d %d", "bank bank"),
  /* 1C */ Command("comparebanktobyte %d %#x", "bank byte"),
  /* 1D */ Command("comparebanktofarbyte %d 0x%08x", "bank ptr"),
  /* 1E */ Command("comparefarbytetobank 0x%08x %d", "ptr bank"),
  /* 1F */ Command("comparefarbytetobyte 0x%08x %#x", "ptr byte"),
  /* 20 */ Command("comparefarbytes 0x%08x 0x%08x", "ptr ptr"),
  /* 21 */ Command("compare %#x %#x", "var word"),
  /* 22 */ Command("comparevars %#x %#x", "var var"),
  /* 23 */ Command("callasm 0x%08x", "ptr"),
  /* 24 */ Command("cmd24 0x%08x", "ptr"),
  /* 25 */ Command("special %#x", "word"),
  /* 26 */ Command("special2 %#x %#x", "var word"),
  /* 27 */ Command("waitstate", ""),
  /* 28 */ Command("pause %#x", "word"),
  /* 29 */ Command("setflag %#x", "flag/var"),
  /* 2A */ Command("clearflag %#x", "flag/var"),
  /* 2B */ Command("checkflag %#x", "flag/var"),
  /* 2C */ Command("cmd2c", ""),
  /* 2D */ Command("checkdailyflags", ""),
  /* 2E */ Command("resetvars", ""),
  /* 2F */ Command("sound %#x", "word"),
  /* 30 */ Command("checksound", ""),
  /* 31 */ Command("fanfare %#x", "word/var"),
  /* 32 */ Command("waitfanfare", ""),
  /* 33 */ Command("playsong %#x %#x", "word byte"),
  /* 34 */ Command("playsong2 %#x", "word"),
  /* 35 */ Command("fadedefault", ""),
  /* 36 */ Command("fadesong %#x", "word"),
  /* 37 */ Command("fadeout %#x", "byte"),
  /* 38 */ Command("fadein %#x", "byte"),
  /* 39 */ Command("warp (bank=%d,map=%d) warp#%d (x=%d,y=%d)", "byte byte byte byte/var byte/var"),
  /* 3A */ Command("warpmuted (bank=%d,map=%d) warp#%d (x=%d,y=%d)", "byte byte byte byte/var byte/var"),
  /* 3B */ Command("warpwalk (bank=%d,map=%d) warp#%d (x=%d,y=%d)", "byte byte byte byte/var byte/var"),
  /* 3C */ Command("warphole %#x %#x", "byte byte"),
  /* 3D */ Command("warpteleport (bank=%d,map=%d) warp#%d (x=%d,y=%d)", "byte byte byte byte/var byte/var"),
  /* 3E */ Command("warp3 (bank=%d,map=%d) warp#%d (x=%d,y=%d)", "byte byte byte byte/var byte/var"),
  /* 3F */ Command("setwarpplace %#x %#x %#x %#x %#x", "byte byte byte word word"),
  /* 40 */ Command("warp4 (bank=%d,map=%d) warp#%d (x=%d,y=%d)", "byte byte byte byte/var byte/var"),
  /* 41 */ Command("warp5 (bank=%d,map=%d) warp#%d (x=%d,y=%d)", "byte byte byte byte/var byte/var"),
  /* 42 */ Command("getplayerpos %#x %#x", "var var"),
  /* 43 */ Command("countPokémon", ""),
  /* 44 */ Command("additem %#x %#x", "word/var byte/var"),
  /* 45 */ Command("removeitem %#x %#x", "word/var byte/var"),
  /* 46 */ Command("checkitemroom %#x %#x", "word/var byte/var"),
  /* 47 */ Command("checkitem %#x %#x", "word/var byte/var"),
  /* 48 */ Command("checkitemtype %#x", "word/var"),
  /* 49 */ Command("addpcitem %#x %#x", "word/var word/var"),
  /* 4A */ Command("checkpcitem %#x %#x", "word/var word/var"),
  /* 4B */ Command("adddecoration %#x", "word/var"),
  /* 4C */ Command("removedecoration %#x", "word/var"),
  /* 4D */ Command("testdecoration %#x", "word/var"),
  /* 4E */ Command("checkdecoration %#x", "word/var"),
  /* 4F */ Command("applymovement %#x 0x%08x", "byte/var ptr"),
  /* 50 */ Command("applymovementpos %#x 0x%08x", "word ptr"),
  /* 51 */ Command("waitmovement %#x", "byte/var"),
  /* 52 */ Command("waitmovementpos %#x %#x %#x", "byte/var byte byte"),
  /* 53 */ Command("hidesprite %#x", "byte/var"),
  /* 54 */ Command("hidespritepos %#x %#x %#x", "byte/var byte byte"),
  /* 55 */ Command("showsprite %#x", "byte/var"),
  /* 56 */ Command("showspritepos %#x %#x %#x", "byte/var byte byte"),
  /* 57 */ Command("", ""),
  /* 58 */ Command("", ""),
  /* 59 */ Command("", ""),
  /* 5A */ Command("faceplayer", ""),
  /* 5B */ Command("", ""),
  /* 5C */ Command("trainerbattle %#x %#x %#x 0x%08x 0x%08x 0x%08x 0x%08x", "byte word word ptr ptr ptr ptr"),
  /* 5D */ Command("repeattrainerbattle", ""),
  /* 5E */ Command("", ""),
  /* 5F */ Command("", ""),
  /* 60 */ Command("checktrainerflag %#x", "word/var"),
  /* 61 */ Command("cleartrainerflag %#x", "word/var"),
  /* 62 */ Command("settrainerflag %#x", "word/var"),
  /* 63 */ Command("movesprite2 %d %d %d", "word word word"),
  /* 64 */ Command("moveoffscreen %#x", "word"),
  /* 65 */ Command("spritebehave %d %d", "word byte"),
  /* 66 */ Command("waitmsg", ""),
  /* 67 */ Command("preparemsg %#x", "ptr/bank0", &Script::_preparemsg),
  /* 68 */ Command("closeonkeypress", ""),
  /* 69 */ Command("lockall", ""),
  /* 6A */ Command("lock", ""),
  /* 6B */ Command("releaseall", ""),
  /* 6C */ Command("release", ""),
  /* 6D */ Command("waitkeypress", ""),
  /* 6E */ Command("yesnobox %#x %#x", "byte byte"),
  /* 6F */ Command("multichoice %#x %#x %#x %#x", "byte byte byte byte"),
  /* 70 */ Command("multichoice2 %#x %#x %#x %#x %#x", "byte byte byte byte byte"),
  /* 71 */ Command("multichoice3 %#x %#x %#x %#x %#x", "byte byte byte byte byte"),
  /* 72 */ Command("", ""),
  /* 73 */ Command("", ""),
  /* 74 */ Command("", ""),
  /* 75 */ Command("showpokepic %#x %#x %#x", "word/var byte byte"),
  /* 76 */ Command("hidepokepic", ""),
  /* 77 */ Command("showcontestwinner %#x", "byte"),
  /* 78 */ Command("braille %#x", "ptr/bank0"),
  /* 79 */ Command("givePokémon %#x %#x %#x %#x %#x %#x", "word/var byte word dword dword byte"),
  /* 7A */ Command("", ""),
  /* 7B */ Command("", ""),
  /* 7C */ Command("checkattack %#x", "word", &Script::_checkattack),
  /* 7D */ Command("bufferPokémon %d %#x", "buffer word/var"),
  /* 7E */ Command("bufferfirstPokémon %d", "buffer"),
  /* 7F */ Command("bufferpartyPokémon %d %#x", "buffer word/var"),
  /* 80 */ Command("bufferitem %d %#x", "buffer word/var"),
  /* 81 */ Command("bufferdecoration %#x", "word/var"),
  /* 82 */ Command("bufferattack %d %#x", "buffer word/var"),
  /* 83 */ Command("buffernumber %d %#x", "buffer word/var"),
  /* 84 */ Command("bufferstd %d %#x", "buffer word/var"),
  /* 85 */ Command("bufferstring %d 0x%08x", "buffer ptr", &Script::_bufferstring),
  /* 86 */ Command("pokemart 0x%08x", "ptr"),
  /* 87 */ Command("pokemart2 0x%08x", "ptr"),
  /* 88 */ Command("pokemart3 0x%08x", "ptr"),
  /* 89 */ Command("", ""),
  /* 8A */ Command("cmd8a", ""),
  /* 8B */ Command("choosecontestpkmn", ""),
  /* 8C */ Command("startcontest", ""),
  /* 8D */ Command("showcontestresults", ""),
  /* 8E */ Command("contestlinktransfer", ""),
  /* 8F */ Command("random %#x", "word/var"),
  /* 90 */ Command("givemoney %#x %#x", "dword byte"),
  /* 91 */ Command("paymoney %#x %#x", "dword byte"),
  /* 92 */ Command("checkmoney %#x %#x", "dword byte"),
  /* 93 */ Command("showmoney %#x %#x", "byte byte"),
  /* 94 */ Command("hidemoney %#x %#x", "byte byte"),
  /* 95 */ Command("updatemoney %#x %#x", "byte byte"),
  /* 96 */ Command("cmd96", ""),
  /* 97 */ Command("fadescreen %#x", "byte"),
  /* 98 */ Command("", ""),
  /* 99 */ Command("", ""),
  /* 9A */ Command("", ""),
  /* 9B */ Command("", ""),
  /* 9C */ Command("doanimation %#x", "word"),
  /* 9D */ Command("setanimation %#x %#x", "byte word/var"),
  /* 9E */ Command("checkanimation %#x", "word"),
  /* 9F */ Command("sethealingplace %#x", "word"),
  /* A0 */ Command("checkgender", ""),
  /* A1 */ Command("cry %#x %#x", "word/var word"),
  /* A2 */ Command("setmaptile %#x %#x %#x %#x", "word word word word"),
  /* A3 */ Command("resetweather", ""),
  /* A4 */ Command("setweather %#x", "word"),
  /* A5 */ Command("doweather", ""),
  /* A6 */ Command("cmda6 %#x", "byte"),
  /* A7 */ Command("", ""),
  /* A8 */ Command("", ""),
  /* A9 */ Command("", ""),
  /* AA */ Command("", ""),
  /* AB */ Command("", ""),
  /* AC */ Command("setdooropened %#x %#x", "word word"),
  /* AD */ Command("setdoorclosed %#x %#x", "word word"),
  /* AE */ Command("doorchange", ""),
  /* AF */ Command("setdooropened2 %#x %#x", "word word"),
  /* B0 */ Command("setdoorclosed2 %#x %#x", "word word"),
  /* B1 */ Command("cmdb1", ""),
  /* B2 */ Command("cmdb2", ""),
  /* B3 */ Command("", ""),
  /* B4 */ Command("", ""),
  /* B5 */ Command("", ""),
  /* B6 */ Command("setwildbattle %#x %#x %#x", "word byte word"),
  /* B7 */ Command("dowildbattle", ""),
  /* B8 */ Command("", ""),
  /* B9 */ Command("", ""),
  /* BA */ Command("", ""),
  /* BB */ Command("", ""),
  /* BC */ Command("", ""),
  /* BD */ Command("", ""),
  /* BE */ Command("", ""),
  /* BF */ Command("", ""),
  /* C0 */ Command("showcoins %#x %#x", "byte byte"),
  /* C1 */ Command("hidecoins", ""),
  /* C2 */ Command("updatecoins %#x %#x", "byte byte"),
  /* C3 */ Command("cmdc3 %d", "hidden"),
  /* C4 */ Command("warp6", ""),
  /* C5 */ Command("waitcry", ""),
  /* C6 */ Command("bufferboxname %d %#x", "buffer word/var"),
  /* C7 */ Command("textcolor %#x", "byte"),
  /* C8 */ Command("cmdc8 %#x", "ptr/bank0"),
  /* C9 */ Command("cmdc9", ""),
  /* CA */ Command("signmsg", ""),
  /* CB */ Command("normalmsg", ""),
  /* CC */ Command("comparehiddenvar %d %#x", "hidden dword"),
  /* CD */ Command("setobedience %#x", "word"),
  /* CE */ Command("checkobedience %#x", "word/var"),
  /* CF */ Command("executeram", ""),
  /* D0 */ Command("setworldmapflag %#x", "flag/var"),
  /* D1 */ Command("warpteleport2", ""),
  /* D2 */ Command("setcatchlocation %#x %#x", "word/var byte"),
  /* D3 */ Command("braille2 %#x", "ptr/bank0"),
  /* D4 */ Command("bufferitems %d %#x %#x", "buffer word/var word/var"),
  /* D5 */ Command("cmdd5", "")
};

static std::string      formatString(const char *s, ...)
{
  va_list	list;
  char          buff[1024];

  va_start(list, s);
  vsnprintf(buff, 1024, s, list);
  va_end(list);
  return (std::string(buff));
}

static std::string      readString(uint32_t ptr)
{
  char		msg[1024];
  uint8_t       *addr = (uint8_t *) gbaMem(ptr);
  int		i;

  for (i = 0; i < 1024 && addr[i] != 0xFF; i++)
    msg[i] = pokeCharsetToAscii(addr[i]);
  msg[i] = '\0';
  return (std::string(msg));
}

void            Script::_loadpointer(Instruction *instr)
{
  if (_ptr[_pc] == 0x09)
    instr->str = formatString("loadpointer %d \"%s\"", instr->args[0], readString(instr->args[1]).c_str());
}

void            Script::_bufferstring(Instruction *instr)
{
  instr->str = formatString("bufferstring %d \"%s\"", instr->args[0], readString(instr->args[1]).c_str());
}

void            Script::_preparemsg(Instruction *instr)
{
  if (instr->args[0] >= 4)
    instr->str = formatString("preparemsg \"%s\"", readString(instr->args[0]).c_str());
}

void            Script::_checkattack(Instruction *instr)
{
  instr->str = formatString("checkattack %s", _data.move(instr->args[0]).getName());
}

void            Script::_if(Instruction *instr)
{
  uint8_t       b = instr->args[0];
  const char    *op = "<\0\0==\0>\0\0<=\0>=\0!=\0?" + (b > 5 ? 18 : b * 3);

  instr->str =
    formatString("if %s ", op) +
    (instr->cmd == 0x06 ? "goto" : "call") +
    formatString(" 0x%08x", instr->args[1]);
  _branch(instr);
}

void            Script::_branch(Instruction *instr)
{
  _addrs.push(instr->args[instr->args.size() - 1]);
}
