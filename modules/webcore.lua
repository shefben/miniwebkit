import("lib.detect.find_program")
import("detect.sdks.find_vstudio")
import("lib.detect.find_tool")


function findvccl()
    local vstudio = find_vstudio()
    for _, vsver in pairs(vstudio) do
        local vcvarsall = vsver.vcvarsall or {}
        for _, arch in ipairs({"x64", "x86"}) do
            local vcvars = vcvarsall[arch]
            if vcvars then
                local tool = find_tool("cl.exe", {force = true, envs = vcvars})
                if tool then
                    return tool.program
                end
            end
        end
    end
    return nil
end
local vccl = findvccl()

function imap(t, iter)
    local result = {}
    for i, v in ipairs(t) do
        table.insert(result, iter(v, i))
    end
    return result
end

function sjoin(arr, delimiter)
    local result = ""
    for i, v in ipairs(arr) do
        if (i == 1) then
            result = v
        else
            result = result .. delimiter .. v
        end
    end
    return result
end

function cldefine(define)
    return "/D "..define
end

function gccdefine(define)
    return "-D"..define
end


function iorun(exec, output)
    local stdout, _ = os.iorun(exec)
    -- print(stderr)
    -- assert (stderr == nil or stderr == "")
    io.writefile(output, stdout)
end

function xxdFile(varname, input, output)
    local INPUT = io.open(input, "r")
    local OUTPUT = io.open(output, "w")
    OUTPUT:writef(";const unsigned char %s[] = {\n", varname)
    local first = true
    while (true) do
        local text = INPUT:read()
        if (text ~= nil) then
            local index = 1
            while(index <= #text) do
                local char = string.byte(text, index)
                if (first) then
                    OUTPUT:writef("%#04x", char)
                    first = false
                else
                    OUTPUT:writef(", %#04x", char)
                end
                index = index+1
            end
            OUTPUT:write(", 0x0a")
        else
            INPUT:close()
            break
        end
    end
    OUTPUT:write("};\n")
    OUTPUT:close()
end

local useCompilerPreprocessor = true

function inlinePreprocessor(input, defines, output)
    local INPUT = io.open(input, "r")
    local OUTPUT = io.open(output, "w")
    local skip = false
    local defineHas = {}
    for _, define in ipairs(defines) do
        defineHas[define] = 1
    end

    while(true) do
        local line = INPUT:read()
        if (line ~= nil) then
            if (line ~= "") then
                if (line:startswith("#")) then
                    if (line:startswith("#if")) then
                        local define = line:match("defined%((.+)%)")
                        if (defineHas[define] == 1) then
                            skip = false
                        else
                            skip = true
                        end
                    elseif (line:startswith("#endif")) then
                        skip = false
                    end
                elseif (not line:startswith("//")) then
                    if (not skip) then
                        OUTPUT:write(line.."\n")
                    end
                end
            end
        else
            INPUT:close()
            break
        end
    end
    OUTPUT:close()
    return
end

function preprocessorFile(input, defines, output)
    local exec = nil
    local msg = ""
    if not useCompilerPreprocessor then
        print("use inlinePreprocessor")
        inlinePreprocessor(input, defines, output)
        return
    elseif (vccl) then
        exec = "\""..vccl.."\" /E /EP "..sjoin(imap(defines, cldefine), " ")
        msg = "cl"
    elseif (find_program("cl") ~= nil) then
        exec = "cl /E /EP "..sjoin(imap(defines, cldefine), " ")
        msg = "cl"
    elseif (find_program("gcc") ~= nil) then
        exec = "gcc -E -P -x c++ "..sjoin(imap(defines, gccdefine), " ")
        msg = "gcc"
    elseif (find_program("tcc") ~= nil) then
        exec = "tcc -E -P -x c++ "..sjoin(imap(defines, gccdefine), " ")
        msg = "tcc"
    else
        print("use inlinePreprocessor")
        inlinePreprocessor(input, defines, output)
        return
    end
    local cmd = exec.." ".. input
    print("use compilerPreprocessor ".. msg)
    iorun(cmd, output)
    return
end

function merge(files, out)
    local fp = io.open(out, "w")
    for _, f in ipairs(files) do
        local data = io.readfile(f)
        fp:write(data)
        fp:write("\n")
    end
    fp:close()
end

function generateGperf(input, output)
    os.run("gperf --key-positions=\"*\" -D -s 2 "..input.." --output-file="..output)
    print("gperf generate "..output.." done")
end


function makevalues(ctx, files)
    local defines = ctx.defines
    local out = ctx.outputDir
    local input = out.."CSSValueKeywords.in"
    merge(files, input)
    preprocessorFile(input, defines, out.."CSSValueKeywords.i")
    input = out.."CSSValueKeywords.i"
    local gperfPath = out.."CSSValueKeywords.gperf"
    local GPERF = io.open(gperfPath, "w")
    local HEADER = io.open(out.."CSSValueKeywords.h", "w")
    local INPUT = io.open(input, "r")
    HEADER:write([[
/* This file is automatically generated from CSSValueKeywords.in by makevalues, do not edit */

#ifndef CSSValueKeywords_h
#define CSSValueKeywords_h

#include <string.h>

namespace WebCore {

const int CSSValueInvalid = 0;
]])

    GPERF:write([[
%{
/* This file is automatically generated from CSSValueKeywords.in by makevalues, do not edit */

#include "CSSValueKeywords.h"
#include "HashTools.h"
#include <string.h>

namespace WebCore {
%}
%struct-type
struct Value;
%omit-struct-type
%language=C++
%readonly-tables
%compare-strncmp
%define class-name CSSValueKeywordsHash
%define lookup-function-name findValueImpl
%define hash-function-name value_hash_function
%define word-array-name value_word_list
%enum
%%
]])
    local namesHash = {}
    local names = {}
    local index = 1
    local maxLen = 0
    while(true) do
        local line = INPUT:read()
        if (line ~= nil) then
            if (line ~= "" and (not line:startswith("//"))) then
                local id = line
                if (not line:startswith("-")) then
                    id = id:gsub("^.", string.upper)
                end
                id = id:gsub("-(.)", string.upper)
                namesHash[line] = id
                table.insert(names, line)
                GPERF:write(line..", CSSValue"..id.."\n")
                HEADER:write("const int CSSValue"..id.." = "..index..";\n")
                index = index + 1
                if (#line > maxLen) then
                    maxLen = #line
                end
            end
        else
            INPUT:close()
            break
        end
    end
    HEADER:write("const int numCSSValueKeywords = "..index..";\n")
    HEADER:write("const size_t maxCSSValueKeywordLength = "..maxLen..";\n")
    HEADER:write([[

const char* getValueName(unsigned short id);

} // namespace WebCore

#endif // CSSValueKeywords_h

]])
    GPERF:write([[
%%
static const char* const valueList[] = {
    "",
]])
    for _, name in ipairs(names) do
        GPERF:write("    \""..name.."\",\n")
    end
    GPERF:write([[
    0
};

const Value* findValue(register const char* str, register unsigned int len)
{
    return CSSValueKeywordsHash::findValueImpl(str, len);
}

const char* getValueName(unsigned short id)
{
    if (id >= numCSSValueKeywords || id <= 0)
        return 0;
    return valueList[id];
}

} // namespace WebCore
]])
    GPERF:close()
    HEADER:close()
    generateGperf(gperfPath, out .."CSSValueKeywords.cpp")
end

function runBison(input, outDir, name, arg)
    os.run("bison -d -p ".. arg .." ".. input .. " -o "..outDir..name..".cpp")
    local header = io.open(outDir..name..".h", "w")
    header:writef("#ifndef %s_h\n", name)
    header:writef("#define %s_h\n", name)
    local cppHeader = io.readfile(outDir..name..".hpp")
    header:write(cppHeader)
    header:write("\n")
    header:write("#endif\n")
    header:close()
    os.tryrm(outDir..name..".hpp")
end


function cssyy(ctx)
    local webCoreDir = ctx.srcDir
    local outDir = ctx.outputDir
    runBison(
        webCoreDir.."css/CSSGrammar.y",
        outDir,
        "CSSGrammar",
        "cssyy"
    )
    print("generate cssyy done")
end


function toPascalCase(s)
    local result = s
    if (not result:startswith("-")) then
        result = result:gsub("^.", string.upper)
    end
    result = result:gsub("-(.)", string.upper)
    return result
end

function subEmpty(s)
    local result = s
    result = result:gsub("^[%s]*([%S]+)[%s]*=[%s]*([%S]+)[%s]*", "%1=%2")
    local arr = result:split("=")
    return arr
end


function ltrim(input)
    return (string.gsub(input, "^[%s]+", ""))
end

function rtrim(input)
    return (string.gsub(input, "[%s]+$", ""))
end

function trim(input)
    return rtrim(ltrim(input))
end


function makeprop(ctx, files)
    local defines = ctx.defines
    local out = ctx.outputDir
    local input = out.."CSSPropertyNames.in"
    merge(files, input)
    preprocessorFile(input, defines, out.."CSSPropertyNames.i")
    input = out.."CSSPropertyNames.i"
    local gperfPath = out.."CSSPropertyNames.gperf"
    local GPERF = io.open(gperfPath, "w")
    local HEADER = io.open(out.."CSSPropertyNames.h", "w")
    local INPUT = io.open(input, "r")

    local aliases = {}
    local names = {}
    while(true) do
        local line = INPUT:read()
        if (line ~= nil) then
            if (line ~= "") then
                if (not line:startswith("//")) then
                    local isAliases = string.find(line, "=") ~= nil
                    if (isAliases) then
                        table.insert(aliases, line)
                    else
                        table.insert(names, line)
                    end
                end
            end
        else
            INPUT:close()
            break
        end
    end
    GPERF:write([[
%{
/* This file is automatically generated from CSSPropertyNames.in by makeprop, do not edit */
#include "CSSPropertyNames.h"
#include "HashTools.h"
#include <string.h>

namespace WebCore {
%}
%struct-type
struct Property;
%omit-struct-type
%language=C++
%readonly-tables
%global-table
%compare-strncmp
%define class-name CSSPropertyNamesHash
%define lookup-function-name findPropertyImpl
%define hash-function-name propery_hash_function
%define word-array-name property_wordlist
%enum
%%
]])
    HEADER:write([[
/* This file is automatically generated from CSSPropertyNames.in by makeprop, do not edit */

#ifndef CSSPropertyNames_h
#define CSSPropertyNames_h

#include <string.h>

namespace WebCore {

enum CSSPropertyID {
    CSSPropertyInvalid = 0,
]])
    local first = 1001
    local index = 1001
    local maxLen = 0
    for _, name in ipairs(names) do
        local key = toPascalCase(name)
        GPERF:write(name .. ", CSSProperty" .. key .. "\n")
        HEADER:write("    CSSProperty" .. key .. " = " .. index .. ",\n")
        index = index+1
        if (#name > maxLen) then
            maxLen = #name
        end
    end
    local num = index - first
    for _, alias in ipairs(aliases) do
        local arr = subEmpty(alias)
        local name = arr[1]
        local key = toPascalCase(arr[2])
        GPERF:write(name .. ", CSSProperty" .. key .."\n")
    end
    GPERF:write([[
%%
const Property* findProperty(register const char* str, register unsigned int len)
{
    return CSSPropertyNamesHash::findPropertyImpl(str, len);
}

const char* getPropertyName(CSSPropertyID id)
{
    if (id < firstCSSProperty)
        return 0;
    int index = id - firstCSSProperty;
    if (index >= numCSSProperties)
        return 0;
    return propertyNameStrings[index];
}

} // namespace WebCore
]])
    HEADER:write("};\n\n")
    HEADER:write("const int firstCSSProperty = "..first..";\n")
    HEADER:write("const int numCSSProperties = "..num..";\n")
    HEADER:write("const size_t maxCSSPropertyNameLength = "..maxLen..";\n")
    HEADER:write("const char* const propertyNameStrings["..num.."] = {\n")
    for _, name in ipairs(names) do
        HEADER:write("\""..name.."\",\n")
    end
    HEADER:write("};\n")
    HEADER:write([[

const char* getPropertyName(CSSPropertyID);

} // namespace WebCore

#endif // CSSPropertyNames_h

]])
    GPERF:close()
    HEADER:close()
    generateGperf(gperfPath, out .."CSSPropertyNames.cpp")
end


function flexProcessor(input, processor, output)
    local flexOutPath = path.join(path.directory(output), path.basename(output)..".flex.cpp")
    iorun("flex -t "..input, flexOutPath)
    iorun("perl "..processor .. " " .. flexOutPath, output)
    os.tryrm(flexOutPath)
    print("flex generate "..output)
end

function makeHTMLEntityTable(ctx)
    local webCoreDir = ctx.srcDir
    local outputDir = ctx.outputDir
    local output = outputDir.."HTMLEntityTable.cpp"
    os.run("python "..webCoreDir.."/html/parser/create-html-entity-table -o "..output.." "..webCoreDir.."/html/parser/HTMLEntityNames.in")
    print("HTMLEntityTable generate "..output)
end


function generateVCEnv()
    local envs = {}
    -- if (vccl) then
    --     envs["CL"] = vccl
    -- end
    return envs
end

function runPerl(webCoreDir, script, defines, args)
    local args_ = {
        "-I"..webCoreDir.."bindings/scripts",
        script
    }
    if (defines) then
        table.insert(args_, "--defines")
        table.insert(args_, sjoin(defines, " "))
    end
    local cmds = table.join(
        args_,
        args
    )
    os.runv("perl", cmds, {envs=generateVCEnv()})
end

function makeUserAgentStyleSheets(ctx, input)
    local defines = ctx.defines
    local webCoreDir = ctx.srcDir
    local outputDir = ctx.outputDir
    local output = outputDir.."UserAgentStyleSheetsData.cpp"
    local outputHeader = outputDir.."UserAgentStyleSheets.h"
    local args = table.join(
        {
            outputHeader,
            output
        },
        input
    )
    runPerl(
        webCoreDir,
        webCoreDir.."css/make-css-file-arrays.pl",
        defines,
        args
    )
    print("UserAgentStyleSheets generate "..output)
end

function makeEventFactory(ctx)
    local defines = ctx.defines
    local webCoreDir = ctx.srcDir
    local outputDir = ctx.outputDir
    local output = outputDir.."EventFactory.cpp"
    local args = {
        output,
        "--outputDir",
        outputDir,
        "--events",
        webCoreDir.."dom/EventFactory.in"
    }
    runPerl(
        webCoreDir,
        webCoreDir.."dom/make_event_factory.pl",
        defines,
        args
    )
    print("EventFactory generate "..output)
end

function makeHTMLElementFactory(ctx)
    local defines = ctx.defines
    local webCoreDir = ctx.srcDir
    local outputDir = ctx.outputDir
    local flags = {}
    local defineHas = {}
    for _, define in ipairs(defines) do
        defineHas[define] = 1
    end
    local checkDefines = {
        "ENABLE_DATALIST",
        "ENABLE_DETAILS",
        "ENABLE_METER_TAG",
        "ENABLE_MICRODATA",
        "ENABLE_PROGRESS_TAG"
    }
    for _, define in ipairs(checkDefines) do
        if (defineHas[define] == 1) then
            table.insert(flags, define.."=1")
        end
    end
    
    local output = outputDir.."HTMLElementFactory.cpp"
    local args = {
        "--tags",
        webCoreDir.."html/HTMLTagNames.in",
        "--attrs",
        webCoreDir.."html/HTMLAttributeNames.in",
        "--outputDir",
        outputDir,
        "--factory",
        "--wrapperFactory"
    }
    if (#flags > 0) then
        table.insert(
            args,
            "--extraDefines"
        )
        table.insert(
            args,
            sjoin(flags, " ")
        )
    end
    runPerl(
        webCoreDir,
        webCoreDir.."dom/make_names.pl",
        nil,
        args
    )
    print("HTMLElementFactory generate "..output)
end


function makeWebKitFontFamilyNames(ctx)
    local outputDir = ctx.outputDir
    local args = {
        "--fonts",
        webCoreDir.."css/WebKitFontFamilyNames.in",
        "--outputDir",
        outputDir
    }
    runPerl(
        webCoreDir,
        webCoreDir.."dom/make_names.pl",
        nil,
        args
    )
    local output = outputDir.."WebKitFontFamilyNames.cpp"
    print("WebKitFontFamilyNames generate "..output)
end

function makeWebCoreHeaderDetection(ctx)
    if (ctx.target:is_plat("windows")) then
        local WEBKITLIBRARIESDIR = os.getenv("WEBKITLIBRARIESDIR") or ""
        local output = ctx.outputDir.."WebCoreHeaderDetection.h"
        local OUTPUT = io.open(output, "w")
        if (os.exists(WEBKITLIBRARIESDIR.."/include/AVFoundationCF/AVCFBase.h")) then
            OUTPUT:write("#define HAVE_AVCF 1\n")
        else
            OUTPUT:write("\n")
        end
        OUTPUT:close()
        print("WebCoreHeaderDetection generate "..output)
    end
end


function generateJSBinding(ctx, input)
    local args = {
        "-Mopen=IO,raw",
        "-I",
        ctx.srcDir.."bindings/scripts",
        ctx.srcDir.."bindings/scripts/generate-bindings.pl",
        "--write-dependencies",
        "--outputDir",
        ctx.outputDir,
        "--defines",
        sjoin(ctx.defines, " ").." LANGUAGE_JAVASCRIPT",
        "--generator",
        "JS"
    }
    for _, includeDir in ipairs({
        "dom",
        "fileapi",
        "html",
        "css",
        "page",
    }) do
        table.insert(args, "--include")
        table.insert(args, ctx.srcDir..includeDir)
    end
    table.insert(args, input)
    os.runv(
        "perl",
        args
    )
end

function makeJSBinding(ctx, inputs)
    local vpath = {
        "",
        "bindings/generic",
        "bindings/objc",
        "css",
        "dom",
        "fileapi",
        "html",
        "html/canvas",
        "loader/appcache",
        "page",
    }
    local srcDir = ctx.srcDir
    for _, f in ipairs(inputs) do
        local file = nil
        for _, dir in ipairs(vpath) do
            local file_ = srcDir..dir.."/"..f..".idl"
            if (os.exists(file_)) then
                file = file_
                break
            end
        end
        assert(file ~= nil)
        generateJSBinding(ctx, file)
        print("JSBinding generate:", ctx.outputDir.."JS"..f..".h")
    end
end

function generateAll(ctx)
    local WebCoreDir = ctx.srcDir
    local WebCoreBuildDir = ctx.outputDir
    local defineHas = ctx.defineHas
    local JS_DOM_IDL = ctx.JS_DOM_IDL
    if (os.exists(WebCoreBuildDir)) then
        return
    end
    print("before build generate")
    os.mkdir(WebCoreBuildDir)
    generateGperf(WebCoreDir.."platform/ColorData.gperf", WebCoreBuildDir.."ColorData.cpp")
    local UserAgentStyleSheetsInput = {
        WebCoreDir.."css/html.css",
        WebCoreDir.."css/quirks.css",
        WebCoreDir.."css/view-source.css",
        WebCoreDir.."css/themeWin.css",
        WebCoreDir.."css/themeWinQuirks.css"
    }
    local makevalueInput = {
         WebCoreDir.."css/CSSValueKeywords.in"
    }
    local makepropInput = {
        WebCoreDir.."css/CSSPropertyNames.in",
    }
    if (defineHas["ENABLE_FULLSCREEN_API"] == 1) then
        table.insert(UserAgentStyleSheetsInput, WebCoreDir.."css/fullscreen.css")
        table.insert(UserAgentStyleSheetsInput, WebCoreDir.."css/fullscreenQuickTime.css")
    end
    if (defineHas["ENABLE_DASHBOARD_SUPPORT"] == 1) then
        table.insert(makepropInput, WebCoreDir.."css/DashboardSupportCSSPropertyNames.in")
    end
    makevalues(
        ctx,
        makevalueInput
    )
    makeprop(
        ctx,
        makepropInput
    )
    flexProcessor(
        WebCoreDir.."css/tokenizer.flex",
        WebCoreDir.."css/maketokenizer",
        WebCoreBuildDir.."tokenizer.cpp"
    )
    cssyy(
        ctx
    )
    makeHTMLEntityTable(
        ctx
    )
    makeUserAgentStyleSheets(
        ctx,
        UserAgentStyleSheetsInput
    )
    makeEventFactory(
        ctx
    )
    makeWebKitFontFamilyNames(
        ctx
    )
    makeWebCoreHeaderDetection(ctx)
    makeJSBinding(ctx, JS_DOM_IDL)
end

function copyHeaders(ctx)
    if (os.exists(ctx.outputDir.."include/WebCore/ForwardingHeaders")) then
        return
    end
    local dirs = {
        "include/WebCore",
        "include/WebCore/ForwardingHeaders",
        "include/WebCore/ForwardingHeaders/bindings",
        "include/WebCore/ForwardingHeaders/parser",
        "include/WebCore/ForwardingHeaders/runtime",
        "include/WebCore/ForwardingHeaders/masm",
        "include/WebCore/ForwardingHeaders/heap",
        "include/WebCore/ForwardingHeaders/wtf",
        "include/WebCore/ForwardingHeaders/wtf/dtoa",
        "include/WebCore/ForwardingHeaders/wtf/text",
        "include/WebCore/ForwardingHeaders/wtf/unicode",
        "include/WebCore/ForwardingHeaders/wtf/unicode/icu",
        "include/WebCore/ForwardingHeaders/yarr",
        "include/WebCore/ForwardingHeaders/interpreter"
    }

    local copyFiles = {
        { "config.h","include/WebCore"},
        { "loader/*.h","include/WebCore"},
        { "loader/appcache/*.h","include/WebCore"},
        { "loader/archive/*.h","include/WebCore"},
        { "loader/archive/cf/*.h","include/WebCore"},
        { "loader/cache/*.h","include/WebCore"},
        { "loader/icon/*.h","include/WebCore"},
        { "html/*.h","include/WebCore"},
        { "html/parser/*.h","include/WebCore"},
        { "css/*.h","include/WebCore"},
        { "platform/*.h","include/WebCore"},
        { "platform/cf/*.h","include/WebCore"},
        { "platform/cf/win/*.h","include/WebCore"},
        { "platform/graphics/*.h","include/WebCore"},
        { "platform/graphics/cairo/*.h","include/WebCore"},
        { "platform/graphics/transforms/*.h","include/WebCore"},
        { "platform/graphics/ca/*.h","include/WebCore"},
        { "platform/graphics/ca/win/*.h","include/WebCore"},
        { "platform/graphics/win/*.h","include/WebCore"},
        { "platform/graphics/win/cairo/*.h","include/WebCore"},
        { "platform/graphics/opentype/*.h","include/WebCore"},
        { "platform/text/*.h","include/WebCore"},
        { "platform/text/transcoder/*.h","include/WebCore"},
        { "platform/win/*.h","include/WebCore"},
        { "platform/network/*.h","include/WebCore"},
        { "platform/network/curl/*.h","include/WebCore"},
        { "platform/sql/*.h","include/WebCore"},
        { "platform/cairo/cairo/src/*.h","include/WebCore"},
        { "bindings/*.h","include/WebCore"},
        { "page/*.h","include/WebCore"},
        { "page/win/*.h","include/WebCore"},
        { "bridge/*.h","include/WebCore"},
        { "bridge/jsc/*.h","include/WebCore"},
        { "rendering/*.h","include/WebCore"},
        { "rendering/style/*.h","include/WebCore"},
        { "editing/*.h","include/WebCore"},
        { "dom/*.h","include/WebCore"},
        { "ForwardingHeaders/bindings/*.h","include/WebCore/ForwardingHeaders/bindings"},
        { "ForwardingHeaders/interpreter/*.h", "include/WebCore/ForwardingHeaders/interpreter" },
        { "ForwardingHeaders/parser/*.h","include/WebCore/ForwardingHeaders/parser"},
        { "ForwardingHeaders/runtime/*.h","include/WebCore/ForwardingHeaders/runtime"},
        { "ForwardingHeaders/masm/*.h","include/WebCore/ForwardingHeaders/masm"},
        { "ForwardingHeaders/heap/*.h","include/WebCore/ForwardingHeaders/heap"},
        { "ForwardingHeaders/wtf/*.h","include/WebCore/ForwardingHeaders/wtf"},
        { "ForwardingHeaders/wtf/dtoa/*.h","include/WebCore/ForwardingHeaders/wtf/dtoa"},
        { "ForwardingHeaders/wtf/text/*.h","include/WebCore/ForwardingHeaders/wtf/text"},
        { "ForwardingHeaders/wtf/unicode/*.h","include/WebCore/ForwardingHeaders/wtf/unicode"},
        { "ForwardingHeaders/wtf/unicode/icu/*.h","include/WebCore/ForwardingHeaders/wtf/unicode/icu"},
        { "ForwardingHeaders/yarr/*.h","include/WebCore/ForwardingHeaders/yarr"}
    }

    for _, dir in ipairs(dirs) do
        os.mkdir(ctx.outputDir..dir)
    end
    os.cp(ctx.outputDir.."obj/WebCore/DerivedSources/*.h", ctx.outputDir.."include/WebCore")
    for _, copyFile in ipairs(copyFiles) do
        os.cp(ctx.srcDir..copyFile[1], ctx.outputDir..copyFile[2])
    end
    print("copy headers done")
end

