
#include<stdlib.h>
#include<pthread.h>

#include<contentsend_interface.h>


volatile const ContentSendInterface* g_contentSendInterface = NULL ;
volatile int g_initContentWidth = 0 ;
volatile int g_initContentHeight = 0;
volatile int g_initContentFramerate = 0;

void contentSendInit(){

}

void contentSendUninit(){


}
