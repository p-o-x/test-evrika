#include <napi.h>
#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")
#include <assert.h>
#include <windows.h> 
#include <lm.h>
#include <string>
#include <shellapi.h>

Napi::Boolean check_user(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::u16string sent_string = info[0].As<Napi::String>().Utf16Value();
    std::wstring my_wstring(sent_string.begin(), sent_string.end());
    LPUSER_INFO_0 pBuf = NULL;
    LPUSER_INFO_0 pTmpBuf;
    DWORD amount, total,i;
    DWORD dwTotalCount = 0;
    amount = NULL;
    total = NULL;
    NetUserEnum(0,
        0,
        FILTER_NORMAL_ACCOUNT,
        (LPBYTE*)&pBuf, 
        MAX_PREFERRED_LENGTH, 
        &amount, 
        &total, 
        NULL);
    int a = 5;
    if ((pTmpBuf = pBuf) != NULL)
    {
        //
        // Loop through the entries.
        //
        for (i = 0; (i < amount); i++)
        {
            assert(pTmpBuf != NULL);
            if (pTmpBuf == NULL)
            {
                fprintf(stderr, "An access violation has occurred\n");
                break;
            }
            LPWSTR val = pTmpBuf->usri0_name;

            if (val == my_wstring) {
                return Napi::Boolean::New(env, true);
            }
            
            pTmpBuf++;
            dwTotalCount++;
        }
    }
    
    return Napi::Boolean::New(env, false);
}

Napi::Boolean launch_browser(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::string address = info[0].As<Napi::String>().Utf8Value();
    LPCWSTR address_formatted = std::wstring(address.begin(), address.end()).c_str();
    ShellExecute(0, 0, address_formatted, 0, 0 , SW_SHOW );
    return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {

    exports.Set(Napi::String::New(env, "check_user"),
                Napi::Function::New(env, check_user));
    exports.Set(Napi::String::New(env, "launch_browser"),
                Napi::Function::New(env, launch_browser));
  return exports;
}

NODE_API_MODULE(test, Init)