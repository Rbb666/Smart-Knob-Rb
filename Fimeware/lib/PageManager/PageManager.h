/*
 * MIT License
 * Copyright (c) 2018-2020 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the follo18wing conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __PAGEMANAGER_H
#define __PAGEMANAGER_H

#include <stdint.h>

class PageManager
{
    typedef void (*CallbackFunction_t)(void *, uint8_t);
    typedef struct PageList
    {
        CallbackFunction_t Callback;
        const char *Name;
    } PageList_t;

public:
    PageManager(uint8_t pageMax, uint8_t pageStackSize = 10);
    ~PageManager();
    uint8_t NowPage, LastPage, NextPage;
    uint8_t NewPage, OldPage;
    typedef enum
    {
        MSG_NONE,
        MSG_Setup,
        MSG_Exit,
        MSG_Loop,
        MSG_MAX
    } MSG_t;

    bool Register(uint8_t pageID, CallbackFunction_t callback, const char *name);
    bool Clear(uint8_t pageID);
    bool Push(uint8_t pageID);
    bool Pop();
    void ChangeTo(uint8_t pageID);
    void EventTransmit(void *obj, uint8_t event);
    void StackClear();
    const char *GetCurrentName();
    void Running();

private:
    PageList_t *PageList;
    uint8_t *PageStack;
    uint8_t PageStackSize;
    uint8_t PageStackTop;
    uint8_t MaxPage;
    bool IsPageBusy;
};

#endif
