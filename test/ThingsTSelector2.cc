#include "FWCore/TFWLiteSelector/test/ThingsTSelector2.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
using namespace std;
#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/TestObjects/interface/ThingCollection.h"
#include "DataFormats/TestObjects/interface/OtherThingCollection.h"

using namespace tfwliteselectortest;

//Names used in common between the worker and the Selector
static const char* kA = "a";
static const char* kRefA = "refA";

void ThingsTSelector2::begin(TList*&)
{
}

void ThingsTSelector2::terminate(TList& out) {
  cout << "terminate" << endl;
  TCanvas * canvas = new TCanvas( );
  out.FindObject(kA)->Draw();
  canvas->SaveAs( "a.jpg" );
  out.FindObject(kRefA)->Draw();
  canvas->SaveAs( "refA.jpg" );
  delete canvas;
}


class ThingsWorker {
  ThingsWorker(const TList*, TList& out ) {
    cout << "begin" << endl;
    h_a  = new TH1F( kA , "a"  , 100,  0, 20 );
    out.Add(h_a);
    
    h_refA  = new TH1F( kA , "refA"  , 100,  0, 20 );
    out.Add(h_refA);
  }
  
  
  
  void process( const edm::Event& iEvent ) {
    cout << "processing event " << endl;
    //  chain->GetEntry( entry );
    using namespace edmtest;
    edm::Handle<OtherThingCollection> hOThings;
    iEvent.getByLabel("OtherThing", "testUserTag", hOThings);
    
    cout << ">> other things found:" << hOThings->size() << endl;
    for ( size_t i = 0; i < hOThings->size(); ++i ) {
      const OtherThing & thing = (*hOThings)[ i ];
      h_refA ->Fill( thing.ref->a );
      cout << ">> ref->a:  " << thing.ref->a <<endl;
    }
    
    edm::Handle<ThingCollection> hThings;
    iEvent.getByLabel("Thing",hThings);
    const ThingCollection& things = *hThings;
    cout << ">> things found:" << things.size() << endl;
    for ( size_t i = 0; i < things.size(); ++i ) {
      const Thing & thing = things[ i ];
      h_a ->Fill( thing.a );
      cout << ">> a:  " << thing.a <<endl;
    }
    
  }
  
  void postProcessing(TList&)
  {
  }
};
