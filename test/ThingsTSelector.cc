#include "FWCore/TFWLiteSelector/test/ThingsTSelector.h"
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

void ThingsTSelector::begin(TList*&)
{
}

void ThingsTSelector::preProcessing(const TList*, TList& out ) {
  cout << "begin" << endl;
  if(0!=h_a) {
     out.Remove(h_a);
     delete h_a;
     h_a=0;
  }
  h_a  = new TH1F( "a" , "a"  , 100,  0, 20 );
  out.Add(h_a);

  if(0!=h_refA) {
     out.Remove(h_refA);
     delete h_refA;
     h_refA=0;
  }
  h_refA  = new TH1F( "refA" , "refA"  , 100,  0, 20 );
  out.Add(h_refA);
}

void ThingsTSelector::process( const edm::Event& iEvent ) {
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

void ThingsTSelector::postProcessing(TList&)
{
}

void ThingsTSelector::terminate(TList& out) {
  cout << "terminate" << endl;
  TCanvas * canvas = new TCanvas( );
  out.FindObject("a")->Draw();
  canvas->SaveAs( "a.jpg" );
  out.FindObject("refA")->Draw();
  canvas->SaveAs( "refA.jpg" );
  delete canvas;
}
