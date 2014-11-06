var fs = require("fs");
var request = require("request");
var cheerio = require("cheerio");

var types = {
    "byte": ["%#x", "byte"],
    "word": ["%#x", "word"],
    "dword": ["%#x", "dword"],
    "pointer": ["0x%08x", "ptr"],
    "variable": ["%#x", "var"],
    "flag": ["%#x", "flag"],
    "bank": ["%d", "bank"],
    "buffer": ["%d", "buffer"],
    "hidden-variable": ["%d", "hidden"],
    "pointer-or-bank": ["%#x", "ptr/bank"],
    "pointer-or-bank-0": ["%#x", "ptr/bank0"],
    "flag-or-variable": ["%#x", "flag/var"],
    "word-or-variable": ["%#x", "word/var"],
    "byte-or-variable": ["%#x", "byte/var"]
};

function makeEntry(idx, format, args)
{
    var out = '  /* ' + (idx < 0x10 ? "0" : "") + idx.toString(16).toUpperCase() + ' */ ';
    out += 'Command("' + format + '", "' + args + '")';
    if (idx != 0xD5)
        out += ',';
    return (out);
}

var cmds = [];
for (var i = 0; i < 0xD6; i++)
    cmds[i] = makeEntry(i, "", "");

request("http://www.sphericalice.co/romhacking/davidjcobb_script/", function(error, response, body) {
    if (!error && response.statusCode == 200) {
        var $ = cheerio.load(body);

        $("ol#list > li").each(function() {
            var title = $(this).find("h3").text().split(" ");
            var num = parseInt(title[0], 16);
            var format = title[1];
            var args = "";
            $(this).find("div.args li > span").each(function() {
                var arg = $(this).text();
                format += " " + types[arg][0];
                args += (args.length > 0 ? " " : "") + types[arg][1];
            });
            cmds[num] = makeEntry(num, format, args);
        });

        console.log('#include        "Script.hh"');
        console.log('');
        console.log('Script::Command Script::_cmds[0xD6] = {');
        for (var i in cmds)
            console.log(cmds[i]);
        console.log('};');
    }
});
