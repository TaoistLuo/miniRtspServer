#include <iostream>
#include "SignalEvent.h"
#include "Mylog.h"

using namespace std;
void test(void* temp){
    LOGI("test SignalEvent\n");

}
int main(){
    SignalEvent tt;
    tt.setCallBack(test,NULL);
    tt.start();
    while(1){
        LOGI("test1\n");
        std::this_thread::sleep_for(std::chrono::microseconds(1*1000*1000));
        LOGI("Set Now");
        tt.setSignal();
    }
}