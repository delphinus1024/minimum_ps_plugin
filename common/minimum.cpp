#include "PIDefines.h"
#include "PIFilter.h"
#include "Logger.h"
#include <algorithm>

// set 0 when log is not needed.
#define DO_DUMP (1)

FilterRecord * gFR = NULL;
int32        * gDataHandle   = NULL;
int16 StartProc( Logger &logIt);
SPBasicSuite*    sSPBasic = NULL; 

#if DO_DUMP
#define printVal(des,v)  logIt.Write (des, false);logIt.Write (":\t", false); logIt.Write (v, true);  
#define printValLn(v) printVal(#v,v);  
#define print(val) logIt.Write(val,false);  
#define println(val) logIt.Write(val,true);  
#else
#define printVal(des,v)
#define printValLn(v)
#define print(val)
#define println(val)
#endif

// dump some important members of FilterRecord, just for study.
void dump_info(Logger &logIt) {
#if DO_DUMP
    if(!gFR)
        return;
        
    printValLn(gFR->imageSize.h);
    printValLn(gFR->imageSize.v);
    printValLn (gFR->maxSpace);  
    printValLn (gFR->planes);  
    printValLn (gFR->depth);  
    printValLn (gFR->filterRect.left);
    printValLn (gFR->filterRect.right);
    printValLn (gFR->filterRect.top);
    printValLn (gFR->filterRect.bottom);
    
    printValLn (gFR->inRect.left);
    printValLn (gFR->inRect.right);
    printValLn (gFR->inRect.top);
    printValLn (gFR->inRect.bottom);
    printValLn (gFR->inLoPlane);  
    printValLn (gFR->inHiPlane);  
    
    printValLn (gFR->outRect.left);
    printValLn (gFR->outRect.right);
    printValLn (gFR->outRect.top);
    printValLn (gFR->outRect.bottom);
    printValLn (gFR->outLoPlane);  
    printValLn (gFR->outHiPlane);  
    
    printValLn (gFR->inRowBytes);  
    printValLn (gFR->outRowBytes);  
    printValLn (gFR->isFloating);  
    printValLn (gFR->haveMask);  
    
    printValLn (gFR->maskRect.left);
    printValLn (gFR->maskRect.right);
    printValLn (gFR->maskRect.top);
    printValLn (gFR->maskRect.bottom);
    printValLn (gFR->maskRowBytes);
    
    printValLn (gFR->imageMode);  
#endif
}

// Entry point
DLLExport MACPASCAL void PluginMain(
    const int16 selector,
    FilterRecord *filterRecord,
    int32 *data,
    int16 *result)
{
    Logger logIt("minimum");

    // update our global parameters
    gFR         = filterRecord;
    gDataHandle = data;

    // do the command according to the selector
    switch (selector)
    {
        case filterSelectorAbout:
            println("filterSelectorAbout called.");
            break;
        case filterSelectorParameters:
            println("filterSelectorParameters called.");
            break;
        case filterSelectorPrepare:
            println("filterSelectorPrepare called.");
            break;
        case filterSelectorStart:
            println("filterSelectorStart called.");
            *result = StartProc(logIt);
            break;
        case filterSelectorContinue:
            println("filterSelectorContinue called.");
            break;
        case filterSelectorFinish:
            println("filterSelectorFinish called.");
            break;
    }
}

// to avoid Visual Studio C2589 error
#undef min

//  main processing
int16 StartProc(  Logger &logIt )
{
    // image size & depth. 
    int16 width  = gFR->filterRect.right  - gFR->filterRect.left;
    int16 height = gFR->filterRect.bottom - gFR->filterRect.top ;
    int16 planes = gFR->planes;

    // set all regions and channels to get whole image
    gFR->inLoPlane = 0;  
    gFR->inHiPlane = planes - 1;  
    gFR->outLoPlane = 0;  
    gFR->outHiPlane = planes - 1;  
    gFR->outRect = gFR->filterRect;
    gFR->inRect  = gFR->filterRect;

    // get image copy
    int16 res = gFR->advanceState();
    if ( res != noErr ) return res;

    println("StartProc called.");
    dump_info(logIt);
    
    // head pointers to source & dest & mask image
    uint8 * inpix; 
    uint8 * outpix; 
    //uint8 * maskpix; // Need study: haveMask is always zero even if mask is set by photoshop.
    
    // process each pixel
    for(int y=0; y < height; y++) {
        inpix   = (uint8 *) gFR->inData + (y * gFR->inRowBytes);
        outpix  = (uint8 *) gFR->outData + (y * gFR->outRowBytes);
        //maskpix = (uint8 *) gFR->maskData + (y * gFR->maskRowBytes);	
        for(int x = 0;x < width;++x) {
            for(int ch = 0;ch < planes;++ch) {
                switch(ch) {
                    case 0: // R
                        outpix[ch] = inpix[ch] / 2;
                        break;
                    case 1: // G
                        outpix[ch] = inpix[ch] / 4;
                        break;
                    case 2: // B
                        outpix[ch] = (uint8)std::min ((uint8)((uint16)inpix[ch] / 2 * 3),(uint8)255);
                        break;
                }
            }
            inpix += planes;
            outpix += planes; 
            //++maskpix;
        }
    }

    // Setting rect to 0 means "processing done"
    memset(&(gFR->outRect),0,sizeof(Rect));
    memset(&(gFR->inRect),0,sizeof(Rect));

    return noErr;
}
