
function copyToBuild(srcDir, outputDir)
    local dirs = {
        "include/JavaScriptCore",
        "include/private/JavaScriptCore",
        "bin/JavaScriptCore.resources"
    }
    local publicHeaders = {
        "APICast.h",
        "APIShims.h",
        "JSBase.h",
        "JSContextRef.h",
        "JSContextRefPrivate.h",
        "JSObjectRef.h",
        "JSObjectRefPrivate.h",
        "JSRetainPtr.h",
        "JSRetainPtr.h",
        "JSStringRef.h",
        "JSStringRefBSTR.h",
        "JSStringRefCF.h",
        "JSValueRef.h",
        "JSWeakObjectMapRefInternal.h",
        "JSWeakObjectMapRefPrivate.h",
        "JavaScript.h",
        "JavaScriptCore.h",
        "OpaqueJSString.h",
        "WebKitAvailability.h"
    }

    local privateHeaders = {
        "assembler",
        "bytecode",
        "dfg",
        "heap",
        "debugger",
        "interpreter",
        "jit",
        "parser",
        "profiler",
        "runtime",
        "wtf",
        "wtf/dtoa",
        "wtf/text",
        "wtf/threads",
        "wtf/unicode",
        "wtf/unicode/icu",
        "yarr"
    }
    local resourceFiles = {
        "JavaScriptCore.resources/*"
    }
    local otherFiles = {
        "create_hash_table",
        "wtf/text/AtomicString.cpp",
        "wtf/text/StringBuilder.cpp",
        "wtf/text/StringImpl.cpp",
        "wtf/text/WTFString.cpp"
    }
    for _, dir in ipairs(dirs) do
        os.mkdir(outputDir..dir)
    end

    for _, copyFile in ipairs(publicHeaders) do
        os.cp(srcDir.."API/"..copyFile, outputDir.."include/JavaScriptCore")
    end
    for _, copyFile in ipairs(privateHeaders) do
        os.cp(srcDir..copyFile.."/*.h", outputDir.."include/private/JavaScriptCore")
    end
    for _, copyFile in ipairs(resourceFiles) do
        os.cp(srcDir..copyFile, outputDir.."bin/JavaScriptCore.resources")
    end
    for _, copyFile in ipairs(otherFiles) do
        os.cp(srcDir..copyFile, outputDir.."include/private/JavaScriptCore")
    end
end

copyToBuild("./src/JavaScriptCore/", "./build/")
