//#include <iostream>

#include "TClonesArray.h"

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "UHH2/bacondataformats/interface/TEventInfo.h"

namespace uhh2bacon {

class JetCorrections {

    private:
    uhh2::Context& context;
    uhh2::Event* event;
   // uhh2::Event::Handle<TClonesArray> h_jetsout;
    uhh2::Event::Handle<TClonesArray> h_jets;
    uhh2::Event::Handle<baconhep::TEventInfo> h_eventInfo;

    int direction = 0; // -1 = down, +1 = up, 0 = nominal

    public:
    JetCorrections(uhh2::Context & ctx);
    ~JetCorrections();

    void SetEvent(uhh2::Event& evt);

    bool JetMatching();
    bool JetResolutionSmearer();
    bool FullJetCorrections();


};

}
